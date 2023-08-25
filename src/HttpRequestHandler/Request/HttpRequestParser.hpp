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
		StateRequestURIStart,
		StateRequestURI,
		StateVersionH,
		StateVersionHT,
		StateVersionHTT,
		StateVersionHTTP,
		StateVersionHTTPSlash,
		StateVersionHTTPVersionMajor,
		StateVersionHTTPVersionDot,
		StateVersionHTTPVersionMinor,
		StateHeaderStart,
		StateHeaderName,
		StateHeaderSpaceAfterName,
		StateHeaderValue,
		StateNewLineCR,
		StateNewLineLF,
	};

  public:

	static void	parseRequest(HttpRequest &request, const char *start, const char *end);
	static void	consume(HttpRequest &request, const char *start, const char *end);
	static void resetParser();
  private:
	static bool isChar(char c);
	static bool isDigit(char c);
	static bool isSpace(char c);
	static bool isControlChar(char c);
	static bool isReserverChar(char c);

	static void	parseRequestMethodStart(HttpRequest &request, char c);
	static void	parseRequestMethod(HttpRequest &request, char c);
	static void	parseRequestUriStart(HttpRequest &request, char c);
	static void	parseRequestUri(HttpRequest &request, char c);
	static void	parseVersionH(HttpRequest &request, char c);
	static void	parseVersionHT(HttpRequest &request, char c);
	static void	parseVersionHTT(HttpRequest &request, char c);
	static void	parseVersionHTTP(HttpRequest &request, char c);
	static void	parseVersionHTTPSlash(HttpRequest &request, char c);
	static void	parseVersionHTTPVersionMajor(HttpRequest &request, char c);
	static void	parseVersionHTTPVersionDot(HttpRequest &request, char c);
	static void	parseVersionHTTPVersionMinor(HttpRequest &request, char c);
	static void parseStateHeaderStart(HttpRequest &request, char c);
	static void parseStateHeaderName(HttpRequest &request, char c);
	static void parseStateSpaceAfterName(HttpRequest &request, char c);
	static void parseStateHeaderValue(HttpRequest &request, char c);
	static void parseStateNewLineCR(HttpRequest &request, char c);
	static void parseStateNewLineLF(HttpRequest &request, char c);


	static ParserResult		_result;
	static ParserState		_state;


};