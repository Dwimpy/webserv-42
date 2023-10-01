#include "BaseResponse.hpp"

class ErrorResponse: public BaseResponse {

  public:
	ErrorResponse(int16_t status_code, const HttpRequest &request, const ConfigFile &config);
	~ErrorResponse();

	void	getContent();

	void pageMethodNotAllowed();
};