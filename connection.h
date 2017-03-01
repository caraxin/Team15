#ifndef CONNECTION_H
#define CONNECTION_H


#include <boost/asio.hpp>
#include <array>
#include <memory>
#include "Request.h"
#include "Response.h"

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
  //for testing purposes
  void setBuffer(std::string& str);
 private:
  void start_reading();
  void read_handler(const boost::system::error_code& ec,std::size_t bytes_transferred);
  void start_writing(std::size_t bytes_transferred);
  void write_handler(const boost::system::error_code& ec,std::size_t bytes_transferred);
  boost::asio::ip::tcp::socket socket_;
  std::array<char,max_length> buffer_;  
  server* server_;

};

  typedef std::shared_ptr<connection> connection_ptr;

}
}

#endif // CONNECTION_H
