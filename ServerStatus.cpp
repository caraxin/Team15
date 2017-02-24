// Based off https://github.com/UCLA-CS130/Team07/blob/master/server_stats.cpp

#include "ServerStatus.h"

namespace Team15 {
namespace server {

ServerStatus& ServerStatus::getInstance() {
  static ServerStatus instance;
  return instance;
}
void ServerStatus::insertHandler(const std::string& prefix, 
        const std::string& handler_name) {
  // Checks for duplicates
  for (unsigned int i=0; i < handlers_.size(); i++) {
    if (handlers_[i].first == prefix) {
      return;
    }
  }

  std::pair<std::string, std::string> handler(prefix, handler_name);
  handlers_.push_back(handler);
}
void ServerStatus::insertRequest(const std::string& uri, 
        RequestHandler::Status status) {
  std::pair<std::string, RequestHandler::Status> request(uri, status);
  requests_.push_back(request);
}
void ServerStatus::resetStatus() {
  handlers_.clear();
  requests_.clear();
}

}
}
