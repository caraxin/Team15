#define TEST_REQUESTMGR

#include "gtest/gtest.h"
#include "requestmgr.h"
#include "Response.h"
namespace Team15 {
namespace server {

TEST(requestmgr, BasicTest) {
  EXPECT_TRUE(true);
}

/*
TEST(requestmgr, BasicEchoTest) {
  std::vector<requestconfig> rc;
  rc.push_back(requestconfig("/echo", "EchoHandler"));
  Team15::server::requestmgr requestmgr(rc);
  char char_array[] = "Basic Echo Test";
  char *body = char_array;
  requestmgr.doServeEcho(body);
  std::unique_ptr<Response> response = requestmgr.generateResponse();
  EXPECT_STREQ(response->toText(),"HTTP/1.0 200 OK\r\nContent-Type: text/plain\r\n\r\nBasic Echo Test") << response->toText();
}

TEST(requestmgr, BasicFileTest) {
  std::vector<requestconfig> rc;
  rc.push_back(requestconfig("/static", "StaticFileHandler"));
  Team15::server::requestmgr requestmgr(rc);
  boost::filesystem::path filePath = "./static/test.html";
  requestmgr.doServeFile(filePath);
  std::unique_ptr<Response> response = requestmgr.generateResponse();
  EXPECT_STREQ(response->toText(),"HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\nStatic File Test") << response->toText();
}
*/

}
}