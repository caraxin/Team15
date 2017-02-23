#include "EchoHandler.h"
namespace Team15 {
namespace server {

  RequestHandler::Status EchoHandler::Init(const std::string& uri_prefix,
		    const NginxConfig& config) {
    return OK;
  }
  RequestHandler::Status EchoHandler::HandleRequest(const Request& request, 
        Response* response) {
    std::string content_length = std::to_string((int) request.body().size());
    response->SetStatus(Response::ResponseCodeOK);
    response->SetReasoning("OK");
    response->AddHeader("Content-Type","text/plain");
    response->AddHeader("Content-Length", content_length);
    response->SetBody(request.body());
    return OK;
  }
}
}
