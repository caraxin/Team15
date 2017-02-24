#include "StaticHandler.h"
#include "Http404Handler.h"
#include "mime_types.hpp"
#include <iterator>
#include <fstream>
namespace Team15 {
namespace server {

  RequestHandler::Status StaticHandler::Init(const std::string& uri_prefix,
		    NginxConfig config) {

    std::string root = "";
    for (auto statement : config.statements_) {
      if (statement->tokens_[0] == "root") {
        root = statement->tokens_[1];
        break;
      }
    }
    if (root == "") {
      // Error no root for file handler
      return RequestHandler::Status::INVALID_INPUT;
    }

    rootPath = root;

    return RequestHandler::Status::OK;
}
  RequestHandler::Status StaticHandler::HandleRequest(const Request& request, 
        Response* response) {
    std::string path = rootPath.string();

    // default file
    if (path[path.size() - 1] == '/') {
      path += "index.html";
    }

    // determine file extension
    std::size_t last_slash_pos = path.find_last_of("/");
    std::size_t last_dot_pos = path.find_last_of(".");
    std::string extension;
    if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos) {
      extension = path.substr(last_dot_pos + 1);
    }

    // open file
    std::ifstream is(path.c_str(), std::ios::in | std::ios::binary);
    if (!is) {
      not_found_handler_->HandleRequest(request, response);
      return NOT_FOUND;
    }
    char c;
    std::string body;
    while (is.get(c)) {
      body += c;
    }
    is.close();

    std::string content_length = std::to_string((int) body.size());
    response->SetStatus(Response::ResponseCodeOK);
    response->SetReasoning("OK");
    response->AddHeader("Content-Type", http::server::mime_types::extension_to_type(extension));
    response->AddHeader("Content-Length", content_length);
    response->SetBody(body);
  
  return RequestHandler::Status::OK;
}

}
}

