#ifndef REQUESTMGR_H
#define REQUESTMGR_H

#include <memory>
#include <vector>
#include <string>
#include "HttpRequest.h"
#include "HttpResponse.h"
#include <boost/filesystem/path.hpp>
namespace Team15 {
namespace server {
  

class requestmgr {
 public:
  requestmgr(std::string fileRoot);
  void handleRequest(std::vector<unsigned char> wire);
  std::unique_ptr<HttpResponse> generateResponse();
 private:
  void doServeEcho(char* body);
  void doServeFile(boost::filesystem::path filePath);
  void doServeError(const std::string& errorCode,const  std::string& reason);
  std::unique_ptr<HttpRequest> request_p;
  std::unique_ptr<HttpResponse> response_p;
  boost::filesystem::path base_file_url_;
};
}
}

#endif
