#include "gtest/gtest.h"

#include "server.h"


TEST(server, BasicTest) {
  Team15::server::server("127.0.0.1","4000");
  EXPECT_TRUE(true);
}
