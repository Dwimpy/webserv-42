#include "GetResponse.hpp"

GetResponse::GetResponse(const HttpRequest &request, const ConfigFile &config): BaseResponse(request, config)
{

	setContentType();
	getContent(_request.getRequestUri());
//		addHeader("Content-Length: ", _)
}

GetResponse::~GetResponse() {}
