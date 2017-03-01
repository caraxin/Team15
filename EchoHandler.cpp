#include "EchoHandler.h"
namespace Team15 {
namespace server {

  RequestHandler::Status EchoHandler::Init(const std::string& uri_prefix,
        NginxConfig config) {
    return RequestHandler::Status::OK;
  }
  RequestHandler::Status EchoHandler::HandleRequest(const Request& request, 
        Response* response) {
    std::string request_body = request.ToString();
    std::string content_length = std::to_string(request_body.size());
    response->SetStatus(Response::ResponseCodeOK);
    response->SetReasoning("OK");
    response->AddHeader("Content-Type", "text/plain");
    response->AddHeader("Content-Length", content_length);
    response->SetBody(request_body);
    return RequestHandler::Status::OK;
  }

}
}
