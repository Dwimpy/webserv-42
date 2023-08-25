#pragma once
#include "HttpRequest.hpp"

class HttpRequestParser{

	enum ParserResult{
		ParserError,
		ParserRunning,
		ParserComplete
	};

	enum ParserState {
		StateRequestMethodStart,
		StateRequestMethod,
	};

  public:

	static void	parseRequest(HttpRequest &request, const char *start, const char *end);
	static void	consume(HttpRequest &request, const char *start, const char *end);

  private:
	bool isChar(char c);
	bool isDigit(char c);
	bool isControlChar(char c);
	bool isReserverChar(char c);
	static ParserResult		_result;
	static ParserState		_state;
};