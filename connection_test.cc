// idea for tests taken from
// https://github.com/UCLA-CS130/spaceteam/blob/master/connection_test.cc

#define TEST_CONNECTION

#include "gtest/gtest.h"
#include "connection.h"
#include "server.h"
#include <boost/asio.hpp>

namespace Team15 {
namespace server {

  TEST(connection, BasicTest) {
    server server("127.0.0.1","4000");
    connection con(boost::asio::ip::tcp::socket(server.getService()),&server);
  }
  TEST(connection, BasicEchoTest) {
    server server("127.0.0.1","4000");
    connection con(boost::asio::ip::tcp::socket(server.getService()),&server);
    std::string buff = "GET TEST";
    con.setBuffer(buff);
    con.generate_response();
    HttpResponse* response = con.getResponse();
    
    //checking the response
    EXPECT_STREQ(response->getStatusCode().c_str(),"200");
    EXPECT_STREQ(response->getReasoning().c_str(),"OK");
    EXPECT_STREQ(response->getBody(),"GET TEST");
  }

  TEST(connection, ReadHandlerSuccess) {
    server server("127.0.0.1","4000");
    connection con(boost::asio::ip::tcp::socket(server.getService()),&server);  
    boost::system::error_code ec = 
      boost::system::errc::make_error_code(boost::system::errc::success);
    EXPECT_TRUE(con.read_handler(ec, 0));
  }
/*
  TEST(connection, ReadHandlerFailure) {
    server server("127.0.0.1","4000"); 
    connection con(boost::asio::ip::tcp::socket(server.getService()),&server);  
    boost::system::error_code ec = 
      boost::system::errc::make_error_code(boost::system::errc::connection_refused);
    EXPECT_FALSE(con.read_handler(ec, 0));
  }

  TEST(connection, WriteHandlerSuccess) {
    server server("127.0.0.1","4000"); 
    connection con(boost::asio::ip::tcp::socket(server.getService()),&server);  
    boost::system::error_code ec = 
      boost::system::errc::make_error_code(boost::system::errc::success);
    EXPECT_TRUE(con.write_handler(ec, 0));
  }  

  TEST(connection, WriteHandlerFailure) {
    server server("127.0.0.1","4000"); 
    connection con(boost::asio::ip::tcp::socket(server.getService()),&server);  
    boost::system::error_code ec = 
      boost::system::errc::make_error_code(boost::system::errc::connection_refused);
    EXPECT_FALSE(con.write_handler(ec, 0));
  }  
*/
}
}

