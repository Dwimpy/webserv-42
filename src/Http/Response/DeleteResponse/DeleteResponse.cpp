#include "DeleteResponse.hpp"

DeleteResponse::DeleteResponse(const HttpRequest &request, const ConfigFile &config):
	BaseResponse(request, config)
{
	_contentType = "text/html";
	deleteResource(request, config);
}

DeleteResponse::~DeleteResponse()
{}

void DeleteResponse::deleteResource(const HttpRequest &request, const ConfigFile &config)
{
	std::string	path;

	path = config.getFilePath(request);
	if (std::remove(path.c_str()) == 0)
		_status_code = 204;
}
