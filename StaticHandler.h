#ifndef STATICHANDLER_H
#define STATICHANDLER_H

#include "RequestHandler.h"
#include <boost/filesystem/path.hpp>
namespace Team15 {
namespace server {

class StaticHandler : public RequestHandler {
 public:
  virtual RequestHandler::Status Init(const std::string& uri_prefix,
		      const NginxConfig& config);
  virtual RequestHandler::Status HandleRequest(const Request& request, Response* response);
 private:
  boost::filesystem::path uri_prefix;
  boost::filesystem::path rootPath;
};
}
}

#endif
