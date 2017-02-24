#ifndef SERVER_STATUS_H
#define SERVER_STATUS_H

#include "RequestHandler.h"
namespace Team15 {
namespace server {

class ServerStatus {
 public:
  static ServerStatus& getInstance();

  void insertHandler(const std::string& prefix, 
          const std::string& handler_name);
  void insertRequest(const std::string& uri, 
          RequestHandler::Status status);
  int getNumRequests() { return requests.size(); }
  void resetStatus();

  std::vector<std::pair<std::string, std::string>> getHandlers() const 
          { return handlers_; }
  std::vector<std::pair<std::string, RequestHandler::Status>> 
          getRequests() const { return requests_; }

 private:
  serverstatus() {} 

  std::vector<std::pair<std::string, std::string>> handlers_;        
  std::vector<std::pair<std::string, RequestHandler::Status>> requests_;
        
};
}
}

#endif // SERVER_STATUS_H
