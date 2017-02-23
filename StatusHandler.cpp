#include "StatusHandler.h"
namespace Team15 {
namespace server {

  RequestHandler::Status StatusHandler::Init(const std::string& uri_prefix,
		    const NginxConfig& config) {
    return OK;
  }
  RequestHandler::Status StatusHandler::HandleRequest(const Request& request, 
        Response* response) {

    return OK;
  }
}
}
