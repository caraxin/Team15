#ifndef STATUSHANDLER_H
#define STATUSHANDLER_H

#include "RequestHandler.h"
namespace Team15 {
namespace server {

class StatusHandler : public RequestHandler {

	virtual RequestHandler::Status Init(const std::string& uri_prefix,
	      const NginxConfig& config);
	virtual RequestHandler::Status HandleRequest(const Request& request 
		  Response* response);

};
}
}

#endif