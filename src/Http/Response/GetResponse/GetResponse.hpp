#pragma once
#include "BaseResponse.hpp"

class GetResponse: public BaseResponse {

  public:
	GetResponse(const HttpRequest &request, const ConfigFile &config);
	~GetResponse();

};