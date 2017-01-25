#ifndef CONNECTION_H
#define CONNECTION_H


#include <boost/asio.hpp>
#include <array>
#include <memory>
#include "HttpRequest.h"
#include "HttpResponse.h"
namespace Team15 {
namespace server {
class connection
{
 public:
  
  explicit connection(boost::asio::ip::tcp::socket socket);
  void start();
  void stop();

 private:
  void start_reading();
  void start_writing();
  boost::asio::ip::tcp::socket socket_;
  std::unique_ptr<HttpRequest> request_p;
  std::unique_ptr<HttpResponse> response_p;
  std::array<char,8192> buffer_;  

};
}
}

#endif
