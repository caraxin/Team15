#include "gtest/gtest.h"

#include "server.h"
#include "requestconfig.h"


TEST(server, BasicTest) {
  std::string addr = "127.0.0.1";
  std::string port = "4000";
  NginxConfig config;
  Team15::server::server server(addr, port, config);
  EXPECT_TRUE(server.is_valid(addr, port));
}

TEST(server, SmallPort) {
  std::string addr = "127.0.0.1";
  std::string port = "1024";
  NginxConfig config;
  Team15::server::server server(addr, port, config);
  EXPECT_TRUE(server.is_valid(addr, port));
}

TEST(server, SmallInvalidPort) {
  std::string addr = "127.0.0.1";
  std::string port = "1023";
  NginxConfig config;
  ASSERT_EXIT(Team15::server::server server(addr, port, config),
  	::testing::ExitedWithCode(1), "Error: Invalid port input");
}

TEST(server, SmallInvalidPort2) {
  std::string addr = "127.0.0.1";
  std::string port = "1";
  NginxConfig config;
  ASSERT_EXIT(Team15::server::server server(addr, port, config),
  	::testing::ExitedWithCode(1), "Error: Invalid port input");
}

TEST(server, LargeInvalidPort) {
  std::string addr = "127.0.0.1";
  std::string port = "65536";
  NginxConfig config;
  ASSERT_EXIT(Team15::server::server server(addr, port, config),
  	::testing::ExitedWithCode(1), "Error: Invalid port input");
}

TEST(server, ZeroPort) {
  std::string addr = "127.0.0.1";
  std::string port = "0";
  NginxConfig config;
  ASSERT_EXIT(Team15::server::server server(addr, port, config),
  	::testing::ExitedWithCode(1), "Error: Invalid port input");
}

TEST(server, NegativePort) {
  std::string addr = "127.0.0.1";
  std::string port = "-1";
  NginxConfig config;
  ASSERT_EXIT(Team15::server::server server(addr, port, config),
  	::testing::ExitedWithCode(1), "Error: Invalid port input");
}

TEST(server, EmptyPort) {
  std::string addr = "127.0.0.1";
  std::string port = "";
  NginxConfig config;
  ASSERT_EXIT(Team15::server::server server(addr, port, config),
  	::testing::ExitedWithCode(1), "Error: Invalid port input");
}

