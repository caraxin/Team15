#include "Http404Handler.h"
namespace Team15 {
namespace server {

  RequestHandler::Status Handler::Init(const std::string& uri_prefix,
		    const NginxConfig& config) {
    return OK;
  }
  RequestHandler::Status Http404Handler::HandleRequest(const Request& request, Response* response) {

    return OK;
  }
}
}
