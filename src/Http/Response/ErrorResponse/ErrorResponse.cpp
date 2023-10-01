#include "ErrorResponse.hpp"
ErrorResponse::ErrorResponse(int16_t statusCode, const HttpRequest &request, const ConfigFile &config): BaseResponse(request, config) {
	  setStatusCode(statusCode);
	  getContent();
}
ErrorResponse::~ErrorResponse() {

}


void ErrorResponse::getContent() {
	switch(_status_code) {
		case 405: pageMethodNotAllowed();
			break ;
	}
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