#include "HttpRequest.h"
#include "HttpResponse.h"
#include "connection.h"

namespace Team15 {
namespace server {
  connection::connection(boost::asio::ip::tcp::socket socket):socket_(std::move(socket)),request_p(),response_p(),buffer_() {
  }
  void connection::start() {
    start_reading();
  }
  void connection::stop() {
    socket_.close();
  }

  void connection::start_reading() {
    //auto self(this->shared_from_this());
    socket_.async_read_some(boost::asio::buffer(buffer_),
      [this]
      (boost::system::error_code ec, std::size_t bytes)
      {
        if (!ec) {
          start_writing();
        }
        
      });
  }
  void connection::start_writing() {
    std::string str(buffer_.begin(),buffer_.end());
    char * wire = new char[str.length()+1];
    strcpy(wire, str.c_str());

    HttpResponse* response = new HttpResponse("", "", wire, str.length()+1);
    response->setStatusCode(OK);
    response->setContentType("text/plain");

    wire = response->toText();
    std::cout << wire << std::endl;

    boost::system::error_code ignored_ec;
    socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
  }
}
}
