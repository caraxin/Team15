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
  void generate_response();
  //for testing purposes
  HttpResponse* getResponse();
  HttpRequest* getRequest();
  void setBuffer(std::string& str);
 private:
  void start_reading();
  bool read_handler(const boost::system::error_code& ec,std::size_t bytes_transferred);
  void start_writing();
  bool write_handler(const boost::system::error_code& ec,std::size_t bytes_transferred);
  boost::asio::ip::tcp::socket socket_;
  std::unique_ptr<HttpRequest> request_p;
  std::unique_ptr<HttpResponse> response_p;
  std::array<char,max_length> buffer_;  
  server* server_;

  // allow tests to access private members
  #ifdef TEST_CONNECTION
  FRIEND_TEST(connection, ReadHandlerSuccess);
  FRIEND_TEST(connection, ReadHandlerFailure);
  FRIEND_TEST(connection, WriteHandlerSuccess);
  FRIEND_TEST(connection, WriteHandlerFailure);
  #endif
};

  typedef std::shared_ptr<connection> connection_ptr;

}
}

#endif
