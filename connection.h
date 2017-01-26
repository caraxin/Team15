#ifndef CONNECTION_H
#define CONNECTION_H


#include <boost/asio.hpp>
#include <array>
#include <memory>
#include "HttpRequest.h"
#include "HttpResponse.h"

enum { max_length = 8192 };

namespace Team15 {
namespace server {
  class server;
class connection
  : public std::enable_shared_from_this<connection>
{
 public:
  
  explicit connection(boost::asio::ip::tcp::socket socket,server* server);
  void start();
  void stop();

 private:
  void start_reading();
  void start_writing();
  boost::asio::ip::tcp::socket socket_;
  std::unique_ptr<HttpRequest> request_p;
  std::unique_ptr<HttpResponse> response_p;
  std::array<char,max_length> buffer_;  
  server* server_;

};

  typedef std::shared_ptr<connection> connection_ptr;

}
}

#endif
