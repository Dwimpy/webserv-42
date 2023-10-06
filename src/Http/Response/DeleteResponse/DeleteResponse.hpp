#include "BaseResponse.hpp"

class DeleteResponse: public BaseResponse {

  public:
	DeleteResponse(const HttpRequest &request, const ConfigFile &config);
	~DeleteResponse();
	void deleteResource(const HttpRequest &request, const ConfigFile &config);
};