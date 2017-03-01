// https://github.com/UCLA-CS130/Team07/blob/master/status_handler.cpp

#include "StatusHandler.h"
#include "ServerStatus.h"
namespace Team15 {
namespace server {

  RequestHandler::Status StatusHandler::Init(const std::string& uri_prefix,
        NginxConfig config) {
    return RequestHandler::Status::OK;
  }
  RequestHandler::Status StatusHandler::HandleRequest(const Request& request, 
        Response* response) {
    std::string body = StatusHandler::createBody();
    std::string content_length = std::to_string((int) body.size());
    response->SetStatus(Response::ResponseCodeOK);
    response->SetReasoning("OK");
    response->AddHeader("Content-Type", "text/html");
    response->AddHeader("Content-Length", content_length);
    response->SetBody(body);
    return RequestHandler::Status::OK;
  }
  std::string StatusHandler::createBody() {
    int numRequests = ServerStatus::getInstance().getNumRequests();
    std::vector<std::pair<std::string, std::string>> handlers = ServerStatus::getInstance().getHandlers();
    std::vector<std::pair<std::string, std::string>> requests = ServerStatus::getInstance().getRequests();

    std::string body = "<html>";
    body += "<head><title>Server Status</title></head>";
    body += "<h2>Handlers</h2>";
    body += "<table><tr><th>URI Prefix</th><th>Request Handler</th></tr>";
    
    for (unsigned int i = 0; i < handlers.size(); i++) {
      body += "<tr><td>" + handlers[i].first + "</td><td>" + handlers[i].second + "</td></tr>"; 
    }

    body += "</table><br>"; 
    body += "<h2>Requests</h2>";
    body += "<p>Number of Requests: " + std::to_string(numRequests) + "</p>" ;
    body += "<table><tr><th>URI Requested</th><th>Response Code</th></tr>";

    for (unsigned int i = 0; i < requests.size(); i++) {
      body += "<tr><td>" + requests[i].first + "</td><td>" + requests[i].second + "</td></tr>"; 
    }

    body += "</table></html>";

    return body;    
  }
}
}
