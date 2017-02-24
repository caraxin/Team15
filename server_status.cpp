#include "server_status.h"

namespace Team15 {
namespace server {

serverstatus& serverstatus::getInstance() {
  static serverstatus instance;
  return instance;
}
void serverstatus::insertHandler(const std::string& prefix, 
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
void serverstatus::insertRequest(const std::string& uri, 
        Response::ResponseCode response) {
  std::pair<std::string, Response::ResponseCode> request(uri, status);
  requests_.push_back(request);
}
void serverstatus::resetStatus() {
  handlers_.clear();
  requests_.clear();
}

}
}
