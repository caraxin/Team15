#include "gtest/gtest.h"
#include "connection.h"
#include "server.h"
#include <boost/asio.hpp>
TEST(connection, BasicTest) {
  Team15::server::server server("127.0.0.1","4000");
  Team15::server::connection con(boost::asio::ip::tcp::socket(server.getService()),&server);
}
TEST(connection, BasicEchoTest) {
  Team15::server::server server("127.0.0.1","4000");
  Team15::server::connection con(boost::asio::ip::tcp::socket(server.getService()),&server);
  std::string buff = "GET TEST";
  con.setBuffer(buff);
  con.generate_response();
  HttpResponse* response = con.getResponse();
  
  //checking the response
  EXPECT_STREQ(response->getStatusCode().c_str(),"200");
  EXPECT_STREQ(response->getReasoning().c_str(),"OK");
  EXPECT_STREQ(response->getBody(),"GET TEST");
}

//will test more with actual parsing logic. For now basic test.
