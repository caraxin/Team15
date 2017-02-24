#include "StatusHandler.h"
#include "ServerStatus.h"
namespace Team15 {
namespace server {

  RequestHandler::Status StatusHandler::Init(const std::string& uri_prefix,
		    const NginxConfig& config) {
    return RequestHandler::Status::OK;
  }
  RequestHandler::Status StatusHandler::HandleRequest(const Request& request, 
        Response* response) {
    std::string body = StatusHandler::createBody();
    std::string content_length = std::to_string((int) body.size());
    response->SetStatus(Response::ResponseCodeNOT_FOUND);
    response->SetReasoning("NOT_FOUND");
    response->AddHeader("Content-Type", "text/plain");
    response->AddHeader("Content-Length", content_length);
    response->SetBody(body);
    return RequestHandler::Status::OK;
  }
  std::string StatusHandler::createBody() {
    std::string body = "";

    return body;    
  }
}
}
