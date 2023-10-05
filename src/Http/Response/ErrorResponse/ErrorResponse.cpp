#include "ErrorResponse.hpp"
ErrorResponse::ErrorResponse(int16_t statusCode, const HttpRequest &request, const ConfigFile &config): BaseResponse(request, config) {
		setStatusCode(statusCode);
		setContentType();
		getContent("/error.rs");
		_contentType = "text/html";
}

ErrorResponse::~ErrorResponse() {

}
