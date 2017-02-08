#include "gtest/gtest.h"

#include "server.h"
#include "requestconfig.h"


TEST(server, BasicTest) {
  std::string addr = "127.0.0.1";
  std::string port = "4000";
  std::vector<requestconfig> rc;
  rc.push_back(requestconfig("/echo", "EchoHandler"));
  Team15::server::server server(addr, port, rc);
  EXPECT_TRUE(server.is_valid(addr, port));
}

TEST(server, SmallPort) {
  std::string addr = "127.0.0.1";
  std::string port = "1024";
  std::vector<requestconfig> rc;
  rc.push_back(requestconfig("/echo", "EchoHandler"));
  Team15::server::server server(addr, port, rc);
  EXPECT_TRUE(server.is_valid(addr, port));
}

TEST(server, SmallInvalidPort) {
  std::string addr = "127.0.0.1";
  std::string port = "1023";
  std::vector<requestconfig> rc;
  rc.push_back(requestconfig("/echo", "EchoHandler"));
  ASSERT_EXIT(Team15::server::server server(addr, port, rc),
  	::testing::ExitedWithCode(1), "Error: Invalid port input");
}

TEST(server, SmallInvalidPort2) {
  std::string addr = "127.0.0.1";
  std::string port = "1";
  std::vector<requestconfig> rc;
  rc.push_back(requestconfig("/echo", "EchoHandler"));
  ASSERT_EXIT(Team15::server::server server(addr, port, rc),
  	::testing::ExitedWithCode(1), "Error: Invalid port input");
}

TEST(server, LargeInvalidPort) {
  std::string addr = "127.0.0.1";
  std::string port = "65536";
  std::vector<requestconfig> rc;
  rc.push_back(requestconfig("/echo", "EchoHandler"));
  ASSERT_EXIT(Team15::server::server server(addr, port, rc),
  	::testing::ExitedWithCode(1), "Error: Invalid port input");
}

TEST(server, ZeroPort) {
  std::string addr = "127.0.0.1";
  std::string port = "0";
  std::vector<requestconfig> rc;
  rc.push_back(requestconfig("/echo", "EchoHandler"));
  ASSERT_EXIT(Team15::server::server server(addr, port, rc),
  	::testing::ExitedWithCode(1), "Error: Invalid port input");
}

TEST(server, NegativePort) {
  std::string addr = "127.0.0.1";
  std::string port = "-1";
  std::vector<requestconfig> rc;
  rc.push_back(requestconfig("/echo", "EchoHandler"));
  ASSERT_EXIT(Team15::server::server server(addr, port, rc),
  	::testing::ExitedWithCode(1), "Error: Invalid port input");
}

TEST(server, EmptyPort) {
  std::string addr = "127.0.0.1";
  std::string port = "";
  std::vector<requestconfig> rc;
  rc.push_back(requestconfig("/echo", "EchoHandler"));
  ASSERT_EXIT(Team15::server::server server(addr, port, rc),
  	::testing::ExitedWithCode(1), "Error: Invalid port input");
}

