#ifndef REQUESTMGR_H
#define REQUESTMGR_H

#include <map>
#include "RequestHandler.h"
#include "nginx-configparser/config_parser.h"
#include <memory>
#include <boost/asio.hpp>
namespace Team15 {
namespace server {

class RequestMgr {
 public:
  RequestMgr(const NginxConfig& config);

  std::unique_ptr<Response> HandleRequest(const std::string& raw_request);
  
 private:
  std::shared_ptr<RequestHandler> getRequestHandler(const std::string& uri);

  void registerPrefix(const std::string& path, 
  					  const std::string& handler, 
  					  const NginxConfig& config);

  std::map<std::string,std::shared_ptr<RequestHandler>> prefixMap;
};
}
}

#endif // REQUESTMGR_H
