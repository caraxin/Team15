#ifndef NOTFOUNDHANDLER_H
#define NOTFOUNDHANDLER_H

#include "RequestHandler.h"
namespace Team15 {
namespace server {

class NotFoundHandler : public RequestHandler {
 public:
	virtual RequestHandler::Status Init(const std::string& uri_prefix,
				NginxConfig config);
	virtual RequestHandler::Status HandleRequest(const Request& request, 
				Response* response);

};

//REGISTER_REQUEST_HANDLER(NotFoundHandler);

}
}

#endif // NOTFOUNDHANDLER_H