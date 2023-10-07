#include "GetResponse.hpp"

GetResponse::GetResponse(const HttpRequest &request, const ConfigFile &config): BaseResponse(request, config)
{
	setContentType();
	std::cout << "request uri " << _request.getRequestUri() << std::endl;
	getContent(_request.getRequestUri());
}

GetResponse::~GetResponse() {}
