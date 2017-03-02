#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H
#include <boost/tokenizer.hpp>
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <stdint.h>
#include <map>
#include <utility>
#include <memory>
#include "HttpMessage.h"

class Request : public HttpMessage
{
 public:
enum RequestMethod {GET};
std::map<RequestMethod, std::string> RequestMethodMap = {{GET,"GET"}};

  //constructors
 static std::unique_ptr<Request> Parse(const std::string& raw_request);

  //get and set uri and method
  std::string uri(void) const { return uri_; }
  void Seturi(const std::string& _uri) { uri_ =_uri; } 
  std::string method(void) const { return method_; }
  void SetMethod(const std::string& _method) { method_ = _method; }
  std::string body(void) const { return body_; }
  void SetBody(const std::string& _body) { body_ = _body; }

  //setting Header fields
  //setting the Connection, a specific header
  std::string ToString(void) const;

 private:
  std::string uri_;
  std::string method_;
  std::string body_;
};

inline std::unique_ptr<Request> Request::Parse(const std::string& raw_request ) {
  typedef boost::tokenizer<boost::char_separator<char>> tokenizer;
  boost::char_separator<char> CRLF{"\r\n"};
  boost::char_separator<char> space{" "};
  
  std::unique_ptr<Request> request = std::unique_ptr<Request>(new Request());
  std::string DCRLF = "\r\n\r\n";
  std::string headerString = raw_request.substr(0,raw_request.find(DCRLF));
  // if have a body
  if (raw_request.size() != raw_request.find(DCRLF) + 4) {
    request->SetBody(
      raw_request.substr(raw_request.find(DCRLF) + 4));
  }

  tokenizer allLines{headerString,CRLF};
  for (tokenizer::iterator it = allLines.begin(); it != allLines.end(); ++it) {
    if (it == allLines.begin()) {
      tokenizer tokens{*it,space};
      tokenizer::iterator it1= tokens.begin();
      request->SetMethod(*it1);
      std::advance(it1,1);
      request->Seturi(*it1);
      std::advance(it1,1);
      request->SetVersion(*it1);
    }
    else {
      std::string::size_type pos = (*it).find(" ");
      if (pos > 0) {
        std::string first = (*it).substr(0,pos);
        first.erase(std::remove(first.begin(),first.end(),':'),first.end());
        for (auto i = HttpHeaderFieldsMap().begin(); i != HttpHeaderFieldsMap().end(); ++i) {
          if (first.compare(i->second)==0) {
            std::string rest = (*it).substr(pos+1);
            request->SetHeaderField(i->first,rest);
            break;
          }
        }
      }
    }
  }
  return request;
    
}

inline std::string Request::ToString(void) const{
  std::string text = method() + " " + uri() + " " + version()+"\r\n";
  std::map<std::string,std::string> map = HeaderFields();
  for(std::map<std::string,std::string>::iterator iter = map.begin(); iter != map.end(); iter++) {
      text+= iter->first + ": " + iter->second + "\r\n";
    }
  text+="\r\n";
  text+=body();
  return text;
}

#endif // HTTP_REQUEST_H
