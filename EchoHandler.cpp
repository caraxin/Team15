#include "EchoHandler.h"
namespace Team15 {
namespace server {

  RequestHandler::Status EchoHandler::Init(const std::string& uri_prefix,
		    const NginxConfig& config) {
    return OK;
  }
  RequestHandler::Status EchoHandler::HandleRequest(const Request& request, Response* response) {
    response->SetStatus(Response::ResponseCodeOK);
    response->SetReasoning("OK");
    response->AddHeader("Content-Type","text/plain");
    response->SetBody(request.body());
    return OK;
  }
}
}
