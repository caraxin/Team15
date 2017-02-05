#include "requestmgr.h"
#include <iterator>
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
  static const std::string ECHO_BASE_URL = "echo";
  static const std::string FILE_BASE_URL = "static";

  requestmgr::requestmgr(std::string fileRoot):request_p(),response_p(),base_file_url_(fileRoot) {}

  void requestmgr::handleRequest(std::vector<unsigned char> wire) {
    request_p.reset(new HttpRequest(wire));
    std::cout << request_p->getUrl() << " " << request_p->getMethod() << " " << std::endl;
    for (unsigned int i = 0; i < wire.size(); ++i) {
      std::cout << wire[i];
    }
    std::cout << std::endl;
    if (request_p->getMethod() == Method_GET) {
      boost::filesystem::path url(request_p->getUrl());
      if (std::distance(url.begin(),url.end()) > 1) {
	boost::filesystem::path root = *(++url.begin());
	if (root.string().compare(ECHO_BASE_URL)==0) {
	  doServeEcho(request_p->toText());
	} else if (root.string().compare(FILE_BASE_URL)==0) {
	  boost::filesystem::path::iterator iterator = url.begin();
	  ++iterator;
	  ++iterator;
	  boost::filesystem::path path;
	  while (iterator != url.end())
	    path /= *iterator;
	  doServeFile(base_file_url_ / path);
	}
	else {
	  doServeError(NOTFOUND_CODE,NOTFOUND_REASON);
	}
      }
      else {
	//empty URL, "/"
	doServeError(NOTFOUND_CODE,NOTFOUND_REASON);
      }
    }
    else {
      doServeError(BADMETHOD_CODE,BADMETHOD_REASON);
    }
    //init request
    //multiplex on URL
    std::cout << response_p->toText() << std::endl;
  }
  std::unique_ptr<HttpResponse> requestmgr::generateResponse() {
    return std::move(response_p);
  }
  void requestmgr::doServeEcho(char* body) {
    //copy body, return OK
    std::unique_ptr<char> wire(new char[strlen(body)+1]);
    strcpy(wire.get(), body);
    response_p.reset(new HttpResponse(OK, "OK", std::move(wire),strlen(body)));
    //wire is now invalid, do not use!!
    response_p->setHeaderField(HttpMessage::HttpHeaderFields::CONTENT_TYPE,("text/plain"));
    
  }
  void requestmgr::doServeFile(boost::filesystem::path filePath) {
    
  }
  void requestmgr::doServeError(const std::string& errorCode, const std::string& reason) {
    response_p.reset(new HttpResponse(errorCode,reason,std::unique_ptr<char>(),0));
  }
}
}
