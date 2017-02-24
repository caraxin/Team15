#include "StatusHandler.h"
namespace Team15 {
namespace server {

  RequestHandler::Status StatusHandler::Init(const std::string& uri_prefix,
		    const NginxConfig& config) {
    return OK;
  }
  RequestHandler::Status StatusHandler::HandleRequest(const Request& request, 
        Response* response) {
    std::string body = createBody();
    std::string content_length = std::to_string((int) body.size());
    response->SetStatus(Response::ResponseCodeNOT_FOUND);
    response->SetReasoning"NOT_FOUND");
    response->AddHeader("Content-Type", "text/plain");
    response->AddHeader("Content-Length", content_length);
    response->SetBody(body);
    return OK;
  }
  std::string createBody() {
    std::string body = "";

    return body;    
  }
}
}
