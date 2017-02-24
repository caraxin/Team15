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
    for (int k = 0; k < (int) config.statements_.size(); k++) { 
      if (config.statements_[k]->tokens_[0] == "default") {
        if (config.statements_[k]->tokens_.size() < 2) {
          // Error incorrect config form; skip this statement
          continue;
        }
        std::string handler = config.statements_[k]->tokens_[1];
        registerPrefix("default", handler);
      }

      if (config.statements_[k]->tokens_[0] == "path") {
        if (config.statements_[k]->tokens_.size() < 3) {
          // Error incorrect config form; skip this statement
          continue;
        }

        std::string path = config.statements_[k]->tokens_[1];
        std::string handler = config.statements_[k]->tokens_[2];
        registerPrefix(path, handler);
      }
    }
    // prefixMap.insert(std::make_pair("/echo",std::make_shared<EchoHandler>()));
    // Error if default handler is not set at this point?
  }
  void RequestMgr::registerPrefix(std::string path, std::string handler) {
    if (prefixMap.find(path) != prefixMap.end()) {
      // Error path is already registered
      return;
    }

    // to add any extra handlers, add the appropriate case statement
    switch(handler) {
      case ECHO_HANDLER:
        prefixMap.insert(std::make_pair(path, std::make_shared<EchoHandler>()));
        break;
      case FILE_HANDLER:
        prefixMap.insert(std::make_pair(path, std::make_shared<StaticHandler>()));
        break;
      default:
        // Error unknown handler specified
    }
  }
  std::shared_ptr<RequestHandler> RequestMgr::getRequestHandler(const std::string& url) {
    std::string current = url;

    while (current != "") {
      if (prefixMap.find(current) != prefixMap.end()) {
        return prefixMap.at(current);
      } else {
        std::size_t pos = current.find_last_of('/');
        if (pos = std::string::npos) break;
        current = current.substr(0, pos);
      }
    }
    // no handler found, return default
    return prefixMap.at("default");
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
