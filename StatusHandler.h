// Based off https://github.com/UCLA-CS130/Team07/blob/master/status_handler.hpp

#ifndef STATUSHANDLER_H
#define STATUSHANDLER_H

#include "RequestHandler.h"
namespace Team15 {
namespace server {

class StatusHandler : public RequestHandler {
 public:
  virtual RequestHandler::Status Init(const std::string& uri_prefix,
        NginxConfig config);
  virtual RequestHandler::Status HandleRequest(const Request& request, 
        Response* response);
 private:
  std::string createBody();
};

//REGISTER_REQUEST_HANDLER(StatusHandler);

}
}

#endif // STATUSHANDLER_H