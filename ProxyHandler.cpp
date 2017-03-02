#include "ProxyHandler.h"
#include "mime_types.hpp"
#include <iterator>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <boost/log/trivial.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

#include <stdlib.h>
#include <ctype.h>

namespace Team15 {

namespace server {

ProxyHandler::ProxyHandler():
  RequestHandler(),
  io_service_()
{
}

RequestHandler::Status
ProxyHandler::Init(const std::string& uri_prefix, NginxConfig config)
{
  uri_prefix_ = uri_prefix;
  for (auto statement: config.statements_) {
      std::string start_token = statement->tokens_[0];
      if (start_token == "server_host") {
        if (statement->tokens_.size() != 2) return RequestHandler::INVALID_INPUT;
        server_host = statement->tokens_[1];
      }
      else if (start_token == "server_port") {
        if (statement->tokens_.size() != 2) return RequestHandler::INVALID_INPUT;
        server_port = statement->tokens_[1];
      }
  }
  return RequestHandler::OK;
}

// sync handleRequest
RequestHandler::Status
ProxyHandler::HandleRequest(const Request& request,
                            Response* response)
{
  // construct the request 
  std::unique_ptr<Request> request_ = CreateProxyRequest(request);
  return IOHandle(server_host, server_port, request_.get(), response);
}

RequestHandler::Status
ProxyHandler::IOHandle(const std::string host,
                       const std::string port,
                       Request* request,
                       Response* response)
{
  std::string raw_request = request->ToString();
  BOOST_LOG_TRIVIAL(trace) << raw_request;

  // resolve the server endpoint
  boost::system::error_code ec;
  boost::asio::ip::tcp::resolver resolver(io_service_);
  boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve({host, port}, ec);
  if (ec) return HandleError("resolve", ec);

  // connect the remmote server
  boost::asio::ip::tcp::socket socket_(io_service_);
  boost::asio::connect(socket_, endpoint_iterator, ec);
  if (ec) return HandleError("connect", ec);
  
  // send the request
  boost::asio::streambuf request_stream_buf;
  std::ostream request_stream(&request_stream_buf);
  request_stream << raw_request;
  boost::asio::write(socket_, request_stream_buf, ec);
  if (ec) return HandleError("write", ec);

  // Read the response status line. The response streambuf will automatically
  // grow to accommodate the entire line. The growth may be limited by passing
  // a maximum size to the streambuf cFonstructor.
  boost::asio::streambuf response_stream_buf;

  boost::asio::read_until(socket_, response_stream_buf, "\r\n", ec);
  if (ec) return HandleError("read_until", ec);

  std::string raw_headers = "";
  // Check that response is OK.
  std::istream response_stream(&response_stream_buf);
  std::string http_version;
  response_stream >> http_version;
  unsigned int status_code;
  response_stream >> status_code;
  std::string status_message;
  std::getline(response_stream, status_message); // contains a space in front of it!
  if (!response_stream || http_version.substr(0, 5) != "HTTP/")
  {
    BOOST_LOG_TRIVIAL(error) << "Invalid response\n";
    // error handler
    return RequestHandler::OK;
  }
  raw_headers += http_version + " " + std::to_string(status_code) + status_message + "\n";

  // Read the response headers, which are terminated by a blank line.
  boost::asio::read_until(socket_, response_stream_buf, "\r\n\r\n", ec);
  if (ec) return HandleError("read_until", ec);

  // Process the 302 code
  std::string header;
  if (status_code == 302) {
    BOOST_LOG_TRIVIAL(trace) << "302 found!";
    while (std::getline(response_stream, header) && header != "\r") {
      if (header.find("Location") != std::string::npos) {
        // delete '/r'
        header.pop_back();
        return HandleRedirect(header, request, response);
      }
    }
    BOOST_LOG_TRIVIAL(error) << "Invalid response\n";
    // error handler
    return RequestHandler::OK;
  }

  // process the headers
  std::string content_length_str = "";
  while (std::getline(response_stream, header) && header != "\r") {
    if (header.find("Content-Length") != std::string::npos) {
      for (auto c: header) {
        if (isdigit(c)) content_length_str += c;
      }
    }
    raw_headers += header + '\n';
  }
  raw_headers += "\r\n";
  response->SetRawHeader(raw_headers);
  if (content_length_str == "") {
    BOOST_LOG_TRIVIAL(error) << "Invalid 200 responses -- without Content-Length Header!";
    // error handler
    return RequestHandler::OK;
  }
  BOOST_LOG_TRIVIAL(trace) << raw_headers;

  // some contents may have been received by read_until
  // according the read_until doc, it may read pass the delimeter
  std::size_t received_content_length = response_stream_buf.size();
  std::size_t left_content_length = boost::lexical_cast<std::size_t>(content_length_str) - received_content_length;
  boost::asio::read(socket_, response_stream_buf,
                    boost::asio::transfer_exactly(left_content_length), ec);
  if (ec) return HandleError("read", ec);

  std::string body;
  std::ostringstream ss;
  ss << &response_stream_buf;
  body = ss.str();
  //HandleContent(body);
  response->SetBody(body);
  return RequestHandler::OK;
}

void
ProxyHandler::HandleContent(const std::string& body)
{/*
  // regex = (?:href|src)=(["'])(?!mailto|https).*?\1
  boost::cmatch ref_mat;
  boost::regex ref_expression ("(href|src)=\"(/.*)\"");
  if (!boost::regex_match(raw_request.c_str(), request_mat, request_expression)) {
    BOOST_LOG_TRIVIAL(info) << "Invalid request format\n";
    return nullptr;
  }*/
}

std::unique_ptr<Request>
ProxyHandler::CreateProxyRequest(const Request& request)
{
  std::unique_ptr<Request> request_ = std::unique_ptr<Request>(new Request(request));
  std::string new_uri = request.uri().substr(uri_prefix_.size());
  if (new_uri == "" || new_uri[0] != '/') new_uri = "/" + new_uri;
  request_->Seturi(new_uri);
  std::map<std::string, std::string> headers = request.GetHeaders();
  for (auto header: headers) {
    if (header.first == "Host") {
      request_->AddHeader("Host", server_host + ":" + server_port);
      break;
    }
  }
  return request_;
}

RequestHandler::Status
ProxyHandler::HandleError(const std::string& error_info, const boost::system::error_code& ec) 
{
  BOOST_LOG_TRIVIAL(trace) << "ProxyHandler: " << error_info << " " << "error_code=" << ec;
  return RequestHandler::OK;
}

RequestHandler::Status
ProxyHandler::HandleRedirect(const std::string& location_header,
                             Request* request,
                             Response *response)
{
  std::string location;
  // absolute url
  if (location_header.find("http://") != std::string::npos) {
    std::string host, port, path, query;
    std::size_t host_start = location_header.find("http://") + 7;
    std::size_t path_start = location_header.find("/", host_start);
    std::string host_and_port;
    // process host and port
    if (path_start == std::string::npos) host_and_port = location_header.substr(host_start);
    else host_and_port = location_header.substr(host_start, path_start - host_start);
    std::size_t delimeter_pos = host_and_port.find(":");
    if (delimeter_pos == std::string::npos 
        || delimeter_pos == host_and_port.size() - 1) {
      host = host_and_port;
      port = "80";
    }
    else {
      host = host_and_port.substr(0, delimeter_pos);
      port = host_and_port.substr(delimeter_pos + 1);
    }
    // process relative_url
    if (path_start == std::string::npos) {
      path = "/";
    }
    else {
      std::string relative_url = location_header.substr(path_start);
      ParsePathAndQuery(relative_url, path, query);
    }
    request->Seturi(path);
    request->SetBody(query);
    // modify Host header
    std::map<std::string, std::string> headers = request->GetHeaders();
    for (auto header: headers) {
      if (header.first == "Host") {
        // because headers is a map, so AddHeader is like SetHeader
        request->AddHeader("Host", host + ":" + port);
        break;
      }
    }
    //std::cout << "host=" << host << " port=" << port << " path=" << path << std::endl;
    return IOHandle(host, port, request, response);
  } // relative url
  else {
    // TODO: maybe multiple spaces after url??
    std::string path;
    std::string query;
    std::size_t path_start = location_header.find_last_of(" ");
    if (path_start == std::string::npos || 
        path_start == location_header.size() - 1) {
      path = "/";
    }
    else {
      std::string relative_url;
      if (location_header[path_start + 1] != '/') 
        relative_url = "/" + location_header.substr(path_start + 1);
      else relative_url = location_header.substr(path_start + 1);
      ParsePathAndQuery(relative_url, path, query);
    }
    request->Seturi(path);
    request->SetBody(query);
    return IOHandle(server_host, server_port, request, response);
  }
  return RequestHandler::OK;
}

void
ProxyHandler::ParsePathAndQuery(const std::string& relative_url,
                                std::string& path,
                                std::string& query)
{
  std::size_t query_pos = relative_url.find("?");
  if (query_pos != std::string::npos) {
    path.append(relative_url.substr(0, query_pos));
    query.append(relative_url.substr(query_pos + 1));
  }
  else path.append(relative_url);
}


} // server
} // Team15