#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include <memory>
#include <vector>
#include <string>
#include "Request.h"
#include "Response.h"
#include "requestconfig.h"
#include "nginx-configparser/config_parser.h"
namespace Team15 {
namespace server {
  

class RequestHandler {
 public:
  enum Status {
    OK = 0,
    INVALID_INPUT = 1
  };
  virtual Status Init(const std::string& uri_prefix,
		      const NginxConfig& config) = 0;
  virtual Status HandleRequest(const Request& request, Response* response) = 0;
 private:
  
  //void doServeEcho(char* body);
  //void doServeFile(boost::filesystem::path filePath);
  //void doServeError(const std::string& errorCode,const  std::string& reason);
  //std::unique_ptr<Request> request_p;
  //std::unique_ptr<Response> response_p;
  //const std::vector<requestconfig>& requestConfigVector_;

  // allow tests to access private members
  //#ifdef TEST_REQUESTMGR
  //FRIEND_TEST(requestmgr, BasicEchoTest);
  //FRIEND_TEST(requestmgr, BasicFileTest);
};
}
}

#endif