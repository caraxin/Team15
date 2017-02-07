#ifndef REQUESTCONFIG_H
#define REQUESTCONFIG_H
#include <string>
class requestconfig {
public:

requestconfig(const std::string& path, const std::string& requestHandler):path_(path),requestHandler_(requestHandler),rootURL_() {}
requestconfig(const std::string& path, const std::string& requestHandler, const std::string& rootURL):path_(path),requestHandler_(requestHandler),rootURL_(rootURL) {}

const std::string getPath() const { return path_; }
const std::string getRequestHandler() const { return requestHandler_; }
const std::string getRootURL() const { return rootURL_; }

private:
std::string path_;
std::string requestHandler_;
std::string rootURL_;
};

#endif
