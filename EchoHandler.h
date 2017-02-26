#ifndef ECHOHANDLER_H
#define ECHOHANDLER_H

#include "RequestHandler.h"
namespace Team15 {
namespace server {

class EchoHandler : public RequestHandler {
 public:
  virtual RequestHandler::Status Init(const std::string& uri_prefix,
        NginxConfig config);
  virtual RequestHandler::Status HandleRequest(const Request& request, 
  		Response* response);

};

//REGISTER_REQUEST_HANDLER(EchoHandler);

}
}

#endif // ECHOHANDLER_H
