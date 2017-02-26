#include "NotFoundHandler.h"
namespace Team15 {
namespace server {

  RequestHandler::Status NotFoundHandler::Init(const std::string& uri_prefix,
		    NginxConfig config) {
    return RequestHandler::Status::OK;
  }
  RequestHandler::Status NotFoundHandler::HandleRequest(const Request& request, 
        Response* response) {
    std::string body = "<!DOCTYPE html><html><body><h1>404 Not Found</h1></body></html>";
    std::string content_length = std::to_string((int) body.size());
    response->SetStatus(Response::ResponseCodeNOT_FOUND);
    response->SetReasoning("NOT_FOUND");
    response->AddHeader("Content-Type", "text/html");
    response->AddHeader("Content-Length", content_length);
    response->SetBody(body);
    return RequestHandler::Status::OK;
  }
}
}
