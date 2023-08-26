#pragma once
#include "string"

class HttpRequestHandler{

	enum Method {
		GET,
		POST,
		DELETE,
	};

  public:
	HttpRequestHandler();
	~HttpRequestHandler();

  private:
	Method	_method;
};