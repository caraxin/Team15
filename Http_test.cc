#include "gtest/gtest.h"
#include "HttpMessage.h"
#include "HttpResponse.h"
#include "HttpRequest.h"

TEST(HttpResponse, BasicBody) {
  const char blankbody[] = {'b','a','s','i','c','b','o','d','y','\0'};
  std::unique_ptr<char> freeablebody(new char[11]());
  strcpy(freeablebody.get(),blankbody);
  HttpResponse response(OK,OK,std::move(freeablebody),9);

  
  EXPECT_STREQ(response.toText(),"HTTP/1.0 200 200 \r\n\r\nbasicbody") << response.toText();
}


TEST(HttpRequest, BasicRequest) {
  HttpRequest request("/","GET");

  
  EXPECT_STREQ(request.toText(),"GET / HTTP/1.0\r\n\r\n") << request.toText();
}


TEST(HttpRequest, IncludingHeader) {
  std::map<std::string,std::string> _headers = { {"Content-length","20"}, {"Date","1/2/3"}, {"Host","cnn.com"}};
  HttpRequest request("/","GET",_headers);
  EXPECT_STREQ(request.toText(),"GET / HTTP/1.0\r\nContent-length: 20\r\nDate: 1/2/3\r\nHost: cnn.com\r\n\r\n") << request.toText();
}

TEST(HttpRequest, TestHttpMessageMethods) {
  HttpRequest request("/","GET");
  request.setVersion("HTTP/1.1");
  EXPECT_STREQ(request.toText(),"GET / HTTP/1.1\r\n\r\n") << request.toText();
  request.setHeaderField(HttpMessage::HttpHeaderFields::CONNECTION,"Keep-alive");
  EXPECT_STREQ(request.toText(),"GET / HTTP/1.1\r\nConnection: Keep-alive\r\n\r\n") << request.toText();
  request.setConnection(HttpMessage::HttpConnectionField::KEEP_ALIVE);
  EXPECT_STREQ(request.toText(),"GET / HTTP/1.1\r\nConnection: keep-alive\r\n\r\n") << request.toText();

}




