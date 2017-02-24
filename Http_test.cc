#include "gtest/gtest.h"
#include "HttpMessage.h"
#include "Response.h"
#include "HttpRequest.h"

TEST(HttpResponse, BasicBody) {
  //test basic response with a body
  std::string body = "basicbody";
  Response response(OK,"OK",body);

  EXPECT_STREQ(response.ToString(),"HTTP/1.0 200 OK\r\n\r\nbasicbody") << response.ToString();
}


TEST(HttpRequest, BasicRequest) {

  //test blank request
  std::unique_ptr<Request>
  
  EXPECT_STREQ(request.ToString(),"GET / HTTP/1.0\r\n\r\n") << request.ToString();
}


TEST(HttpRequest, IncludingHeader) {

  //test initialization with header fields
  std::map<std::string,std::string> _headers = { {"Content-length","20"}, {"Date","1/2/3"}, {"Host","cnn.com"}};
  HttpRequest request("/","GET",_headers);
  EXPECT_STREQ(request.ToString(),"GET / HTTP/1.0\r\nContent-length: 20\r\nDate: 1/2/3\r\nHost: cnn.com\r\n\r\n") << request.ToString();
}

TEST(HttpRequest, TestHttpMessageMethods) {
  HttpRequest request("/","GET");

  //test set and get Version
  request.setVersion("HTTP/1.1");
  EXPECT_STREQ(request.getVersion().c_str(),"HTTP/1.1");
  EXPECT_STREQ(request.ToString(),"GET / HTTP/1.1\r\n\r\n") << request.ToString();

  //test set and get HeaderFields
  request.setHeaderField(HttpMessage::HttpHeaderFields::CONNECTION,"Keep-alive");
  EXPECT_STREQ(request.getHeaderField(HttpMessage::HttpHeaderFields::CONNECTION).c_str(),"Keep-alive");
  EXPECT_STREQ(request.ToString(),"GET / HTTP/1.1\r\nConnection: Keep-alive\r\n\r\n") << request.ToString();

  //test set Connection
  request.setConnection(HttpMessage::HttpConnectionField::KEEP_ALIVE);
  EXPECT_STREQ(request.getHeaderField(HttpMessage::HttpHeaderFields::CONNECTION).c_str(),"keep-alive");
  EXPECT_STREQ(request.ToString(),"GET / HTTP/1.1\r\nConnection: keep-alive\r\n\r\n") << request.ToString();

}

TEST(HttpRequest, testHttpRequestMethods) {
   HttpRequest request("/","GET");


   //test set and get URL
   request.setUrl("/index.html");
   EXPECT_STREQ(request.getUrl().c_str(),"/index.html");
   EXPECT_STREQ(request.ToString(),"GET /index.html HTTP/1.0\r\n\r\n") << request.ToString();

   //test set and get Method
   request.setMethod("POST");
   EXPECT_STREQ(request.getMethod().c_str(),"POST");
   EXPECT_STREQ(request.ToString(),"POST /index.html HTTP/1.0\r\n\r\n") << request.ToString();

}

TEST(HttpRequest, parseFromWire) {
  //test sample request from raw string
  std::string requestStr = "GET / Http/1.1\r\nHost: 127.0.0.1:4000\r\nConnection: keep-alive\r\nCache-Control: max-age=0\r\nUpgrade-Insecure-Requests: 1\r\nUser-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/56.0.2924.76 Safari/537.36\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\nAccept-Encoding: gzip, deflate, sdch, br\r\nAccept-Language: en-GB,en-US;q=0.8,en;q=0.6\r\n\r\n";
  std::vector<unsigned char> wire;
  for (auto it = requestStr.begin(); it != requestStr.end(); ++it) {
    wire.push_back(*it);
  }
  HttpRequest request(wire);
  EXPECT_STREQ(request.getMethod().c_str(),"GET");
  EXPECT_STREQ(request.getUrl().c_str(),"/");
  EXPECT_STREQ(request.getVersion().c_str(),"Http/1.1");
  //the reason this is six is because of what we currently analyze, could change
  EXPECT_EQ(request.getHeaderFields().size(),6) << "Didn't recognize 6 fields"; 
}

TEST(HttpResponse,testHttpResponseMethods) {
  std::unique_ptr<char> freeablebody(new char[1]());
  freeablebody.get()[0]='\0';
  HttpResponse response(OK,OK,std::move(freeablebody),0);
  

  //test setStatusCode, getStatusCode
  response.setStatusCode("300");
  EXPECT_STREQ(response.getStatusCode().c_str(),"300");
  EXPECT_STREQ(response.ToString(),"HTTP/1.0 300 200\r\n\r\n");
  

  //test getReasoning, setReasoning
  response.setReasoning("OK");
  EXPECT_STREQ(response.getReasoning().c_str(),"OK");
  EXPECT_STREQ(response.ToString(),"HTTP/1.0 300 OK\r\n\r\n");
  

  //test getBody, setBody, getBodyLength
  std::unique_ptr<char> newBody(new char[20]());
  std::string bodyString = "bodyString";
  strcpy(newBody.get(),bodyString.c_str());
  response.setBody(std::move(newBody),bodyString.size());
  EXPECT_STREQ(response.getBody(),bodyString.c_str());
  EXPECT_STREQ(response.ToString(),"HTTP/1.0 300 OK\r\n\r\nbodyString");

}

