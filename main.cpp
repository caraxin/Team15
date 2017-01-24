
#include "server.h"
#include <iostream>
#include <boost/version.hpp>
int main(int argc, char** argv) {
  Team15::server::server s = Team15::server::server("localhost","80");
  
  s.run();

  std::cout << "BOOST VERSION: " << BOOST_VERSION/100000 << "." << BOOST_VERSION/100 % 1000<< "." << BOOST_VERSION% 100 << std::endl;
  return 0;
}
