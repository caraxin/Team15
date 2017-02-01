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
 HttpResponse(std::string _code, std::string _reason, std::unique_ptr<char> _body,int _bodyLength) : HttpMessage(),statuscode(_code),reasoning(_reason),body(std::move(_body)),bodyLength(_bodyLength),deallocFlag(0) {}
   HttpResponse(char * wire);
  std::string getStatusCode(void) const { return statuscode; }
  void setStatusCode(std::string _code) { statuscode=_code; }
  std::string getReasoning(void) const { return reasoning; }
  void setReasoning(std::string r) { reasoning = r; }
  char* getBody(void) const { return body.get(); }
  int getBodyLength(void) const {return bodyLength; }
  void setBody(std::unique_ptr<char> b, int length) { body.reset(b.release()); bodyLength = length; }
  void setContentType(std::string c) { contentType = c; }
  
  //setting Header fields
  
  char* toText(void);
  std::string getHeaderText(void);
 private:
  std::string statuscode;
  std::string reasoning;
  std::string contentType;
  std::unique_ptr<char> toTextBody;
  std::unique_ptr<char> body;
  int bodyLength;
  
  int deallocFlag;

};

inline std::string HttpResponse::getHeaderText(void) {
  std::string text = getVersion() + " " + statuscode + " " + contentType + " " + reasoning + "\r\n";
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

inline HttpResponse::HttpResponse(char * wire) : HttpMessage() {
  typedef boost::tokenizer<boost::char_separator<char>> tokenizer;
  std::string s(wire);
  boost::char_separator<char> CRLF{"\r\n"};
  boost::char_separator<char> space{" "};
  deallocFlag = 1;
  reasoning="";
  statuscode="";
  std::string DCRLF = "\r\n\r\n";
  std::string str1 = s.substr(0,s.find(DCRLF));
  tokenizer allLines{str1,CRLF};
  //std::cout <<"About to look at lines " << std::distance(allLines.begin(),allLines.end()) << "\n";
  for (tokenizer::iterator it = allLines.begin(); it != allLines.end(); ++it) {
    if (it == allLines.begin()) {
      //std::cout << "Looking at first line" << "\n";
      tokenizer tokens{*it,space};
      tokenizer::iterator it1= tokens.begin();
      //std::cout << "First " <<*it1<< "\n";
      setVersion(*it1);
      if (std::distance(tokens.begin(),tokens.end()) > 1) {
	std::advance(it1,1);
	//	std::cout << "Second" <<*it1<< "\n";
	statuscode = *it1;
	if (std::distance(tokens.begin(),tokens.end()) > 2) {
	  std::advance(it1,1);
	  //  std::cout << "Third" <<*it1<< "\n";
	  reasoning = *it1;
	}
      }
    }
    else {
      std::string::size_type pos = (*it).find(" ");
      if (pos > 0) {
	std::string first = (*it).substr(0,pos);
	first.erase(std::remove(first.begin(),first.end(),':'),first.end());
	for (auto i = HttpHeaderFieldsMap.begin(); i != HttpHeaderFieldsMap.end(); ++i) {
	  if (first.compare(i->second)==0) {
	    	std::string rest = (*it).substr(pos+1);
		setHeaderField(i->first,rest);
	    break;
	  }
	}

      }
    }
  }
   std::map<std::string,std::string>  map = getHeaderFields();
  if (map.find(HttpHeaderFieldsMap[CONTENT_LENGTH]) != map.end() ) {
    std::string str_ = map[HttpHeaderFieldsMap[CONTENT_LENGTH]];
    int contentLength = atoi(str_.c_str());
    std::unique_ptr<char> b( new char[contentLength]());
    memcpy(b.get(),wire+str1.length()+DCRLF.length(),contentLength);
    body.reset(b.release());
    bodyLength = contentLength;
    deallocFlag = 1;
  }
}

#endif

