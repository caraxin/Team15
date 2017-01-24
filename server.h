#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>

#include <string>

namespace Team15 {
namespace server {

class server
{
  //Constructor
  server(const std::string& address, const std::string& port);

  // Run the server's io loop
  void run();

};
}
}
#define
