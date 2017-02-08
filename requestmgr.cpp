// idea for doServeFile taken from
// http://www.boost.org/doc/libs/1_61_0/doc/html/boost_asio/example/cpp03/http/server/request_handler.cpp

#include "requestmgr.h"
#include "mime_types.hpp"
#include <iterator>
#include <fstream>

namespace Team15{
namespace server{
  //Http Methods
  static const std::string Method_GET = "GET";

  //Error codes and reasons
  static const std::string BADREQUEST_CODE = "400";
  static const std::string BADREQUEST_REASON = "Bad Request";
  static const std::string NOTFOUND_CODE = "404";
  static const std::string NOTFOUND_REASON = "File Not Found";
  static const std::string BADMETHOD_CODE = "405";
  static const std::string BADMETHOD_REASON = "Method Not Allowed";
  
  //Root URLS
  static const std::string ECHO_HANDLER = "EchoHandler";
  static const std::string FILE_HANDLER = "StaticFileHandler";

  requestmgr::requestmgr(const std::vector<requestconfig>& r):request_p(),response_p(),requestConfigVector_(r) {}

  void requestmgr::handleRequest(std::vector<unsigned char> wire) {
    request_p.reset(new HttpRequest(wire));
    if (request_p->getMethod() == Method_GET) {
      boost::filesystem::path url(request_p->getUrl());
      if (std::distance(url.begin(),url.end()) > 1) {
      	boost::filesystem::path root = *(url.begin()) / *(++url.begin());
      	std::string requestHandler;
      	std::string rootURL;
      	for (const requestconfig& rc : requestConfigVector_) {
      	  if (rc.getPath().compare(root.string()) == 0) {
      	    requestHandler = rc.getRequestHandler();
      	    rootURL = rc.getRootURL();
      	  }
      	}
      	if (requestHandler.compare(ECHO_HANDLER)==0) {
      	  doServeEcho(request_p->toText());
      	} else if (requestHandler.compare(FILE_HANDLER)==0) {
      	  boost::filesystem::path::iterator iterator = url.begin();
      	  ++iterator;
      	  ++iterator;
      	  boost::filesystem::path path;
      	  while (iterator != url.end()) {
      	    path /= *iterator;
      	    ++iterator;
      	  }
      	  doServeFile(boost::filesystem::path(rootURL) / path);
      	}
      	else {
      	  doServeError(NOTFOUND_CODE,NOTFOUND_REASON);
      	}
            }
            else {
      	//empty URL, "/"
      	doServeError(NOTFOUND_CODE,NOTFOUND_REASON);
            }
          }
          else {
            doServeError(BADMETHOD_CODE,BADMETHOD_REASON);
          }
  }

  std::unique_ptr<HttpResponse> requestmgr::generateResponse() {
    return std::move(response_p);
  }
  void requestmgr::doServeEcho(char* body) {
    //copy body, return OK
    std::unique_ptr<char> wire(new char[strlen(body)+1]);
    strcpy(wire.get(), body);
    response_p.reset(new HttpResponse(OK, "OK", std::move(wire),strlen(body)));
    //wire is now invalid, do not use!!
    response_p->setHeaderField(HttpMessage::HttpHeaderFields::CONTENT_TYPE,("text/plain"));
  }
  void requestmgr::doServeFile(boost::filesystem::path filePath) {
    std::string path = filePath.string();

    // default file
    if (path[path.size() - 1] == '/') {
      path += "index.html";
    }

    // determine file extension
    std::size_t last_slash_pos = path.find_last_of("/");
    std::size_t last_dot_pos = path.find_last_of(".");
    std::string extension;
    if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos) {
      extension = path.substr(last_dot_pos + 1);
    }

    // open file
    std::ifstream is(path.c_str(), std::ios::in | std::ios::binary);
    if (!is) {
      doServeError(NOTFOUND_CODE,NOTFOUND_REASON);
      return;
    }
    char c;
    std::string str;
    while (is.get(c)) {
      str += c;
    }
    is.close();
    std::unique_ptr<char> wire(new char[str.length()]);
    strcpy(wire.get(), str.c_str());

    // generate response
    response_p.reset(new HttpResponse(OK, "OK", std::move(wire),str.length()));
    //wire is now invalid, do not use!!
    response_p->setHeaderField(HttpMessage::HttpHeaderFields::CONTENT_TYPE,http::server::mime_types::extension_to_type(extension));
  }
  void requestmgr::doServeError(const std::string& errorCode, const std::string& reason) {
    response_p.reset(new HttpResponse(errorCode,reason,std::unique_ptr<char>(),0));
  }
}
}
