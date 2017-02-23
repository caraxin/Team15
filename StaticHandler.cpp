#include "StaticHandler.h"
namespace Team15 {
namespace server {

  RequestHandler::Status StaticHandler::Init(const std::string& uri_prefix,
		    const NginxConfig& config) {
    return OK;
}
  RequestHandler::Status StaticHandler::HandleRequest(const Request& request, Response* response) {
  /*std::string path = filePath.string();

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
      doServeError(NOTFOUND_CODE,NOTFOUND_REASON);
      return;
    }
    char c;
    std::string str;
    while (is.get(c)) {
      str += c;
    }
    is.close();
    std::unique_ptr<char> wire(new char[str.length()]);
    strcpy(wire.get(), str.c_str());

    // generate response
    response_p.reset(new HttpResponse(OK, "OK", std::move(wire),str.length()));
    //wire is now invalid, do not use!!
    response_p->setHeaderField(HttpMessage::HttpHeaderFields::CONTENT_TYPE,http::server::mime_types::extension_to_type(extension));
  */
  return OK;
}

}
}

