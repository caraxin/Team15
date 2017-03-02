// Based off https://github.com/UCLA-CS130/Team07/blob/master/status_handler.hpp

#ifndef TEST_HANDLER_H
#define TEST_HANDLER_H

#include "RequestHandler.h"
namespace Team15 {
namespace server {

class TestHandler1 : public RequestHandler {
 public:
  virtual RequestHandler::Status Init(const std::string& uri_prefix,
        NginxConfig config);
  virtual RequestHandler::Status HandleRequest(const Request& request, 
        Response* response);
};

class TestHandler2 : public RequestHandler {
 public:
  virtual RequestHandler::Status Init(const std::string& uri_prefix,
        NginxConfig config);
  virtual RequestHandler::Status HandleRequest(const Request& request, 
        Response* response);
};

RequestHandler::Status 
TestHandler1::Init(const std::string& uri_prefix, NginxConfig config) 
{
    return RequestHandler::Status::OK;
}
  
RequestHandler::Status 
TestHandler1::HandleRequest(const Request& request, Response* response) {
    std::string request_body = "302 Relative_path Redirect!";
    std::string content_length = std::to_string(request_body.size());
    response->Setstatus("302");
    response->SetReasoning("Found");
    response->AddHeader("Location", "/static");
    response->SetBody(request_body);
    return RequestHandler::Status::OK;
 }

RequestHandler::Status 
TestHandler2::Init(const std::string& uri_prefix, NginxConfig config) 
{
    return RequestHandler::Status::OK;
}
  
RequestHandler::Status 
TestHandler2::HandleRequest(const Request& request, Response* response) {
    std::string request_body = "302 Absolut_path Redirect!";
    std::string content_length = std::to_string(request_body.size());
    response->Setstatus("302");
    response->SetReasoning("Found");
    response->AddHeader("Location", "http://localhost:8080/static");
    response->SetBody(request_body);
    return RequestHandler::Status::OK;
 } 

//REGISTER_REQUEST_HANDLER(StatusHandler);

}
}

#endif // STATUSHANDLER_H