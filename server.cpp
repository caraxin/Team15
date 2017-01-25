#include <ctime>
#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#include "server.h"


std::string make_daytime_string()
{
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}

namespace Team15 {
namespace server {

  server::server(const std::string& address, const std::string& port):
    io_service_(),socket_(io_service_), acceptor_(io_service_){
    boost::asio::ip::tcp::resolver resolver(io_service_);
    boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve({address, port});
    acceptor_.open(endpoint.protocol());
    acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint);
    acceptor_.listen();
    start_accepting();
  }
  void server::start_accepting() {
    std::cout << "Accepting connections" << std::endl;
    acceptor_.async_accept(socket_,
			   [this](boost::system::error_code ec)
			   {
			     if (!acceptor_.is_open()) { std::cerr << "Failed to open" << std::endl; return; }
			     if (!ec) {
			       connection_ptr c_p = std::make_shared<connection>(std::move(socket_));
			       connections_.insert(c_p);
			       c_p->start();
			     }
			     else {
			       std::cerr << "Found error" << std::endl;
			     }
			     start_accepting();
			   });
  }
  void server::run() {
    /*	  try
	  {
	    boost::asio::io_service io_service;

	    tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 13));

	    for (;;)
	    {
	      tcp::socket socket(io_service);
	      acceptor.accept(socket);

	      std::string message = make_daytime_string();

	      boost::system::error_code ignored_error;
	      boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
	    }
	  }
	  catch (std::exception& e)
	  {
	    std::cerr << e.what() << std::endl;
	  }

	  return;*/
    io_service_.run();
  }

}
}
