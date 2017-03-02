#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H
#include "HttpMessage.h"
#include <string>
#include <boost/tokenizer.hpp>
#include <memory>

class Response : public HttpMessage
{
 public:
  enum ResponseCode {
    ResponseCodeOK = 200,
    ResponseCodeBAD_REQUEST = 400,
    ResponseCodeNOT_FOUND = 404
  };
 Response(void): HttpMessage() {}
 Response(std::string _code, std::string _reason, std::string _body) : HttpMessage(),statuscode_(_code),reasoning_(_reason),body_(_body){}
  std::string statuscode(void) const { return statuscode_; }
  void SetStatus(const ResponseCode response_code) { statuscode_=ResponseCodeMap().at(response_code); }
  void Setstatus(const std::string& status) { statuscode_ = status;}
  std::string reasoning(void) const { return reasoning_; }
  void SetReasoning(std::string r) { reasoning_ = r; }
  std::string body(void) const { return body_; }
  void SetBody(std::string _body) { body_ = _body; }
  void SetRawHeader(const std::string& header) { raw_header = header; }
  //setting Header fields
  
  virtual std::string ToString(void) const;
  std::string header_text(void) const;
 private:
  static const std::map<ResponseCode, std::string>& ResponseCodeMap(void) { static const std::map<Response::ResponseCode, std::string> ResponseCodeMap = {{ResponseCodeOK, "200"},{ResponseCodeBAD_REQUEST,"400"},{ResponseCodeNOT_FOUND, "404"}}; return ResponseCodeMap; } 
  std::string version_;
  std::string statuscode_;
  std::string reasoning_;
  std::string body_;

  std::string raw_header;
};

inline std::string Response::header_text(void) const {
  std::string text = version() + " " + statuscode() + " " + reasoning() + "\r\n";
  auto headerFields = HeaderFields();
  for(auto iter = headerFields.begin(); iter != headerFields.end(); ++iter)
    {
      text+= iter->first + ": " + iter->second + "\r\n";
    }
  text += "\r\n";
  return text;
}

inline std::string Response::ToString(void) const {
  if (raw_header.size() == 0) return header_text() + body();
  else return raw_header + body();
}

#endif // HTTP_RESPONSE_H

