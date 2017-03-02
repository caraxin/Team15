#ifndef HTTP_MESSAGE_H
#define HTTP_MESSAGE_H
#include <string>
#include <map>
#include <vector>
#include <cstring>
#include <boost/foreach.hpp>

class HttpMessage
{
 public:

  enum HttpHeaderFields {CONTENT_LENGTH,DATE,CONTENT_TYPE,HOST,USER_AGENT,FROM,CONNECTION,ACCEPT_ENCODING,ACCEPT_LANGUAGE,ACCEPT}; //continued
enum HttpConnectionField {KEEP_ALIVE, CLOSE};
 static const std::map<HttpHeaderFields, std::string>& HttpHeaderFieldsMap(void) { static const std::map<HttpMessage::HttpHeaderFields, std::string> HttpHeaderFieldsMap= {{CONTENT_LENGTH,"Content-Length"},{DATE,"Date"},{CONTENT_TYPE,"Content-Type"},{HOST,"Host"},{USER_AGENT,"User-Agent"},{FROM,"From"},{CONNECTION,"Connection"},{ACCEPT_ENCODING,"Accept-Encoding"},{ACCEPT_LANGUAGE,"Accept-Language"},{ACCEPT,"Accept"}}; return HttpHeaderFieldsMap; }


const std::string HttpVersionToken = "HTTP/1.0";


 HttpMessage(void) {httpVersion_ = HttpVersionToken;}
 HttpMessage(const std::map<std::string,std::string>& _headers) { httpVersion_ = HttpVersionToken; headerFields_=_headers; }
 std::string version(void) const { return httpVersion_; }
 void SetVersion(const std::string& _version) { httpVersion_ =_version; } 
 void SetHeaderField(const HttpHeaderFields key, const std::string& value) { AddHeader(HttpHeaderFieldsMap().at(key),value); }
 
 std::string FetchHeaderField(HttpHeaderFields key) { return headerFields_[(HttpHeaderFieldsMap().at(key))]; }
 void AddHeader(const std::string& header_name, const std::string& header_value) {
   headerFields_[header_name] = header_value;
 }

 void SetConnection(HttpConnectionField conn) { SetHeaderField(CONNECTION,(conn==KEEP_ALIVE?std::string("keep-alive"):std::string("close"))); }

 std::map<std::string, std::string> GetHeaders() const{
 	return headerFields_;
 }



  virtual std::string ToString(void) const=0;
  
  std::map<std::string,std::string> HeaderFields(void) const {return headerFields_;}
  std::vector<std::pair<std::string,std::string>> headers() const { return std::vector<std::pair<std::string,std::string>>(headerFields_.begin(),headerFields_.end()); }
 protected:
  std::string getHttpVersion(void) {return httpVersion_;}

 private:   //private vars
  std::string httpVersion_;
  std::map<std::string,std::string> headerFields_;
};

#endif
