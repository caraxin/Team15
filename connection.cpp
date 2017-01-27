#include "HttpRequest.h"
#include "HttpResponse.h"
#include "connection.h"
#include "server.h"
namespace Team15 {
namespace server {
  connection::connection(boost::asio::ip::tcp::socket socket,server* server):socket_(std::move(socket)),request_p(),response_p(),buffer_(),server_(server) {
  }
  void connection::start() {
    start_reading();
  }
  void connection::stop() {
    socket_.close();
  }

  void connection::start_reading() {
   
    socket_.async_read_some(boost::asio::buffer(buffer_),
			    [this]
      (boost::system::error_code ec, std::size_t bytes)
      {
        if (!ec) {
          start_writing();
        }
	else {
	  server_->connection_done(this);
	}
        
      });
  }
  void connection::start_writing() {
   
    std::string str(buffer_.begin(),buffer_.end());
    std::unique_ptr<char> wire(new char[str.length()+1]);
    strcpy(wire.get(), str.c_str());

    response_p.reset(new HttpResponse("", "", wire.get(), str.length()+1));
    response_p->setStatusCode(OK);
    response_p->setContentType("text/plain");
    
    //std::cout << wire.get() << std::endl;
    const std::string response_buf(response_p->toText());
    boost::asio::async_write(socket_,boost::asio::buffer(response_buf),
			     [this](boost::system::error_code ec, std::size_t bytes_transferred) {
			       if (!ec) {
				 boost::system::error_code ignored_ec;
				 socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
			       }
			       server_->connection_done(this);
			     });

  }
}
}
