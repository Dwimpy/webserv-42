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
		StateEndRequest,
		StateBodyStart,
	};

  public:

	static void	parseRequest(HttpRequest &request, const unsigned char *start, const unsigned char *end);
	static void	consume(HttpRequest &request, const unsigned char *start, const unsigned char *end);
	static void resetParser();
  private:
	static bool isChar(unsigned char c);
	static bool isDigit(unsigned char c);
	static bool isSpace(unsigned char c);
	static bool isControlChar(unsigned char c);
	static bool isReserverChar(unsigned char c);

	static void	parseRequestMethodStart(HttpRequest &request, unsigned char c);
	static void	parseRequestMethod(HttpRequest &request, unsigned char c);
	static void	parseRequestUriStart(HttpRequest &request, unsigned char c);
	static void	parseRequestUri(HttpRequest &request, unsigned char c);
	static void	parseVersionH(HttpRequest &request, unsigned char c);
	static void	parseVersionHT(HttpRequest &request, unsigned char c);
	static void	parseVersionHTT(HttpRequest &request, unsigned char c);
	static void	parseVersionHTTP(HttpRequest &request, unsigned char c);
	static void	parseVersionHTTPSlash(HttpRequest &request, unsigned char c);
	static void	parseVersionHTTPVersionMajor(HttpRequest &request, unsigned char c);
	static void	parseVersionHTTPVersionDot(HttpRequest &request, unsigned char c);
	static void	parseVersionHTTPVersionMinor(HttpRequest &request, unsigned char c);
	static void parseStateHeaderStart(HttpRequest &request, unsigned char c);
	static void parseStateHeaderName(HttpRequest &request, unsigned char c);
	static void parseStateSpaceAfterName(HttpRequest &request, unsigned char c);
	static void parseStateHeaderValue(HttpRequest &request, unsigned char c);
	static void parseStateNewLineCR(HttpRequest &request, unsigned char c);
	static void parseStateNewLineLF(HttpRequest &request, unsigned char c);
	static void	parseStateEndRequest(HttpRequest &request, unsigned char c);
	static void parseStateBodyStart(HttpRequest &request, unsigned char c);
	static ParserResult		_result;
	static ParserState		_state;


};