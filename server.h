#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>

#include <string>

namespace Team15 {
namespace server {

class server
{
 public:
  //Constructor
  server(const server&) = delete;
  server& operator=(const server&) = delete;

  explicit server(const std::string& address, const std::string& port);

  // Run the server's io loop
  void run();
 private:
  void start_accepting();
  boost::asio::io_service io_service_;
  boost::asio::ip::tcp::socket socket_;
  boost::asio::ip::tcp::acceptor acceptor_;

};
}
}
#endif
