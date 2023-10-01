#include "GetResponse.hpp"

GetResponse::GetResponse(const HttpRequest &request, const ConfigFile &config): BaseResponse(request, config)
{
		getContent();
//		addHeader("Content-Length: ", _)
}

GetResponse::~GetResponse() {}



void GetResponse::getContent()
{
//		addHeader("Cache-Control", "max-age=3600");
}
