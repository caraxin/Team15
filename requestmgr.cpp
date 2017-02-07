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
  static const std::string ECHO_HANDLER = "EchoHandler";
  static const std::string FILE_HANDLER = "StaticFileHandler";

  requestmgr::requestmgr(const std::vector<requestconfig>& r):request_p(),response_p(),requestConfigVector_(r) {}

  void requestmgr::handleRequest(std::vector<unsigned char> wire) {
    request_p.reset(new HttpRequest(wire));
    if (request_p->getMethod() == Method_GET) {
      boost::filesystem::path url(request_p->getUrl());
      if (std::distance(url.begin(),url.end()) > 1) {
	boost::filesystem::path root = *(url.begin()) / *(++url.begin());
	std::string requestHandler;
	std::string rootURL;
	for (const requestconfig& rc : requestConfigVector_) {
	  if (rc.getPath().compare(root.string()) == 0) {
	    requestHandler = rc.getRequestHandler();
	    rootURL = rc.getRootURL();
	  }
	}
	if (requestHandler.compare(ECHO_HANDLER)==0) {
	  doServeEcho(request_p->toText());
	} else if (requestHandler.compare(FILE_HANDLER)==0) {
	  boost::filesystem::path::iterator iterator = url.begin();
	  ++iterator;
	  ++iterator;
	  boost::filesystem::path path;
	  while (iterator != url.end()) {
	    path /= *iterator;
	    ++iterator;
	  }
	  doServeFile(boost::filesystem::path(rootURL) / path);
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
    //replace this with creating a response with a file
    doServeError(NOTFOUND_CODE,NOTFOUND_REASON);
    
  }
  void requestmgr::doServeError(const std::string& errorCode, const std::string& reason) {
    response_p.reset(new HttpResponse(errorCode,reason,std::unique_ptr<char>(),0));
  }
}
}
