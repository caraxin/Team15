// idea for doServeFile taken from
// http://www.boost.org/doc/libs/1_61_0/doc/html/boost_asio/example/cpp03/http/server/request_handler.cpp

#include "RequestMgr.h"
#include "mime_types.hpp"
#include <iterator>
#include <fstream>
#include "EchoHandler.h"
#include "StaticHandler.h"
namespace Team15{
namespace server{
  //Http Methods
  static const std::string Method_GET = "GET";

  //Error codes and reasons
  static const std::string BADREQUEST_CODE = "400";
  static const std::string BADREQUEST_REASON = "Bad Request";
  static const std::string NOTFOUND_CODE = "404";
  static const std::string NOTFOUND_REASON = "File Not Found";
  static const std::string BADMETHOD_CODE = "405";
  static const std::string BADMETHOD_REASON = "Method Not Allowed";
  
  //Root URLS
  static const std::string ECHO_HANDLER = "EchoHandler";
  static const std::string FILE_HANDLER = "StaticFileHandler";

  RequestMgr::RequestMgr(const NginxConfig& config) {
    //setup Prefix Map
    //Area for Byron
    prefixMap.insert(std::make_pair("/echo",std::make_shared<EchoHandler>()));
  }
  std::shared_ptr<RequestHandler> RequestMgr::getRequestHandler(const std::string& url) {
    //do prefix logic here
    //Byron?
    return prefixMap.at("/echo");
  }
  std::unique_ptr<Response> RequestMgr::HandleRequest(const std::string& raw_request) {
    std::unique_ptr<Request> request_p = Request::Parse(raw_request);
    auto handler = getRequestHandler(request_p->uri());
    auto response = std::unique_ptr<Response>(new Response());
    RequestHandler::Status status = handler->HandleRequest(*request_p.get(),response.get());
    if (status != RequestHandler::Status::OK) {
      //Error
    }
    return response;
  }
      
}
}
