#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H
#include "HttpMessage.h"
#include <string>
#include <boost/tokenizer.hpp>
#include <memory>
const std::string OK = "200";
const std::string BAD_REQUEST = "400";
const std::string NOT_FOUND = "404";

class HttpResponse : public HttpMessage
{
 public:
 HttpResponse(std::string _code, std::string _reason, std::unique_ptr<char> _body,int _bodyLength) : HttpMessage(),statuscode(_code),reasoning(_reason),body(std::move(_body)),bodyLength(_bodyLength){}
  std::string getStatusCode(void) const { return statuscode; }
  void setStatusCode(std::string _code) { statuscode=_code; }
  std::string getReasoning(void) const { return reasoning; }
  void setReasoning(std::string r) { reasoning = r; }
  char* getBody(void) const { return body.get(); }
  int getBodyLength(void) const {return bodyLength; }
  void setBody(std::unique_ptr<char> b, int length) { body.reset(b.release()); bodyLength = length; }
  //setting Header fields
  
  char* toText(void);
  std::string getHeaderText(void);
 private:
  std::string statuscode;
  std::string reasoning;
  std::unique_ptr<char> toTextBody;
  std::unique_ptr<char> body;
  int bodyLength;
};

inline std::string HttpResponse::getHeaderText(void) {
  std::string text = getVersion() + " " + statuscode + " " + reasoning + "\r\n";
  std::map<std::string,std::string> headerFields = getHeaderFields();
  for(std::map<std::string,std::string>::iterator iter = headerFields.begin(); iter != headerFields.end(); ++iter)
    {
      text+= iter->first + ": " + iter->second + "\r\n";
    }
  text += "\r\n";
  return text;
}

inline char* HttpResponse::toText(void) {

  std::string text = getHeaderText();
  
  toTextBody.reset(new char[(text.length() + bodyLength+1)]());
  strcpy(toTextBody.get(),text.data());
  
  if (body) {
    memcpy(toTextBody.get()+text.length(),body.get(),bodyLength);
  }
  toTextBody.get()[text.length()+bodyLength]='\0';
  return toTextBody.get();
}

#endif

