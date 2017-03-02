#ifndef PROXY_HANDLER_H
#define PROXY_HANDLER_H

#include "RequestHandler.h"
#include <boost/asio.hpp>

namespace Team15 {
namespace server {
  
class ProxyHandler : public RequestHandler {
 public:
  ProxyHandler();

  RequestHandler::Status Init(const std::string& uri_prefix,
                              NginxConfig config);

  RequestHandler::Status HandleRequest(const Request& request, 
                                       Response* response);

  void HandleContent(const std::string& body);
  
  RequestHandler::Status IOHandle(const std::string host,
                                  const std::string port,
                                  Request* request,
                                  Response* response);

  std::unique_ptr<Request> CreateProxyRequest(const Request& request);

  RequestHandler::Status HandleError(const std::string& error_info, 
                                     const boost::system::error_code& ec);

  RequestHandler::Status HandleRedirect(const std::string& location_header,
                                        Request* request,
                                        Response *response);

  void ParsePathAndQuery(const std::string& relative_url,
                         std::string& path,
                         std::string& query);

 private:
  boost::asio::io_service io_service_;

  std::string uri_prefix_;

  std::string server_host;

  std::string server_port;

  const std::size_t max_length = 8192; 

};


} // server
} // Team15

#endif // PROXY_HANDLER_H