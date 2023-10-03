#include "ErrorResponse.hpp"
ErrorResponse::ErrorResponse(int16_t statusCode, const HttpRequest &request, const ConfigFile &config): BaseResponse(request, config) {
		setStatusCode(statusCode);
		setContentType();
		getContent("/error.rs");
		addHeader("Content-type", "text/html");
		addHeader("Content-length", std::to_string(_content.length()));
}

ErrorResponse::~ErrorResponse() {

}


void	ErrorResponse::pageMethodNotAllowed()
{
	std::string allowed_methods;

	_content = "Method Not Allowed: The requested method is not supported.";
	addHeader("Allow", _config.getAllowedMethods());
	addHeader("Content-type", "text/html");
	addHeader("Content-length", std::to_string(_content.length()));
//	appendFileContents("./docs/error_pages/404.html");
}