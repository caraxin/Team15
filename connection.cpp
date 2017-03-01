#include "Request.h"
#include "Response.h"
#include "connection.h"
#include "server.h"
#include <boost/bind.hpp>
namespace Team15 {
namespace server {
  
  connection::connection(boost::asio::ip::tcp::socket socket,server* server):socket_(std::move(socket)),buffer_(),server_(server) {
  }
  void connection::start() {
    start_reading();
  }
  void connection::stop() {
    socket_.close();
  }

  void connection::start_reading() {
    socket_.async_read_some(boost::asio::buffer(buffer_),
          boost::bind(&connection::read_handler,
          this,boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
  }
  void connection::read_handler(const boost::system::error_code& ec,std::size_t bytes_transferred) {
    if (!ec) {
      start_writing(bytes_transferred);
    }
    else {
      server_->connection_done(this);
    }
  }

  void connection::start_writing(std::size_t bytes_transferred) {
    // what do you want to do? buffer_.end() ?????
    //std::unique_ptr<Response> response = server_->getRequestMgr().HandleRequest(std::string(buffer_.begin(),buffer_.end()));
    std::string raw_request = std::string(buffer_.data(), buffer_.data() + bytes_transferred);
    std::unique_ptr<Response> response = server_->getRequestMgr().HandleRequest(raw_request);
    const std::string response_buf(response->ToString());
    boost::asio::async_write(socket_,boost::asio::buffer(response_buf),
           boost::bind(&connection::write_handler,
           this,boost::asio::placeholders::error,
           boost::asio::placeholders::bytes_transferred));
  }
  void connection::write_handler(const boost::system::error_code& ec,std::size_t bytes_transferred) {
    if (!ec) {
      boost::system::error_code ignored_ec;
      socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
    }
    server_->connection_done(this);
  }
  void connection::setBuffer(std::string& str) {
    std::array<char,max_length> newBuff;
    for (unsigned int i = 0; i < str.length(); ++i) {
      newBuff[i] = str[i];
    }
    buffer_ = newBuff;
  }
}
}
