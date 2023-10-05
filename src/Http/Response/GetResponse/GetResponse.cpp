#include "GetResponse.hpp"

GetResponse::GetResponse(const HttpRequest &request, const ConfigFile &config): BaseResponse(request, config)
{

	setContentType();
	getContent(_request.getRequestUri());
}

GetResponse::~GetResponse() {}
