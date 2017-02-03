#include "gtest/gtest.h"

#include "server.h"


TEST(server, BasicTest) {
  std::string addr = "127.0.0.1";
  std::string port = "4000";
  Team15::server::server server_(addr, port);
  EXPECT_TRUE(server_.is_valid(addr, port));
}

TEST(server, SmallPort) {
  std::string addr = "127.0.0.1";
  std::string port = "1024";
  Team15::server::server server_(addr, port);
  EXPECT_TRUE(server_.is_valid(addr, port));
}

TEST(server, SmallInvalidPort) {
  std::string addr = "127.0.0.1";
  std::string port = "1023";
  ASSERT_EXIT(Team15::server::server server_(addr, port),
  	::testing::ExitedWithCode(1), "Error: Invalid port input");
}

TEST(server, SmallInvalidPort2) {
  std::string addr = "127.0.0.1";
  std::string port = "1";
  ASSERT_EXIT(Team15::server::server server_(addr, port),
  	::testing::ExitedWithCode(1), "Error: Invalid port input");
}

TEST(server, LargeInvalidPort) {
  std::string addr = "127.0.0.1";
  std::string port = "65536";
  ASSERT_EXIT(Team15::server::server server_(addr, port),
  	::testing::ExitedWithCode(1), "Error: Invalid port input");
}

TEST(server, ZeroPort) {
  std::string addr = "127.0.0.1";
  std::string port = "0";
  ASSERT_EXIT(Team15::server::server server_(addr, port),
  	::testing::ExitedWithCode(1), "Error: Invalid port input");
}

TEST(server, NegativePort) {
  std::string addr = "127.0.0.1";
  std::string port = "-1";
  ASSERT_EXIT(Team15::server::server server_(addr, port),
  	::testing::ExitedWithCode(1), "Error: Invalid port input");
}

TEST(server, EmptyPort) {
  std::string addr = "127.0.0.1";
  std::string port = "";
  ASSERT_EXIT(Team15::server::server server_(addr, port),
  	::testing::ExitedWithCode(1), "Error: Invalid port input");
}

