#include "HttpRequestParser.hpp"
#include "HttpRequest.hpp"
#include <_ctype.h>

HttpRequestParser::ParserState HttpRequestParser::_state = StateRequestMethodStart;
HttpRequestParser::ParserResult HttpRequestParser::_result = ParserRunning;

void HttpRequestParser::consume(HttpRequest &request, const char *start, const char *end)
{
	while (start != end && HttpRequestParser::_result != ParserError)
	{
		char c = *start++;
		switch (HttpRequestParser::_state)
		{
			case StateRequestMethodStart: parseRequestMethodStart(request, c);
				break;
			case StateRequestMethod: parseRequestMethod(request, c);
				break;
			case StateRequestURIStart: parseRequestUriStart(request, c);
				break;
			case StateRequestURI: parseRequestUri(request, c);
				break;
			case StateVersionH: parseVersionH(request, c);
				break;
			case StateVersionHT: parseVersionHT(request, c);
				break;
			case StateVersionHTT: parseVersionHTT(request, c);
				break;
			case StateVersionHTTP: parseVersionHTTP(request, c);
				break;
			case StateVersionHTTPSlash: parseVersionHTTPSlash(request, c);
				break;
			case StateVersionHTTPVersionMajor: parseVersionHTTPVersionMajor(request, c);
				break;
			case StateVersionHTTPVersionDot: parseVersionHTTPVersionDot(request, c);
				break;
			case StateVersionHTTPVersionMinor: parseVersionHTTPVersionMinor(request, c);
				break;
			case StateNewLineCR: parseStateNewLineCR(request, c);
				break;
			case StateNewLineLF: parseStateNewLineLF(request, c);
				break;
			case StateHeaderStart: parseStateHeaderStart(request, c);
				break;
			case StateHeaderName: parseStateHeaderName(request, c);
				break ;
			case StateHeaderSpaceAfterName: parseStateSpaceAfterName(request, c);
				break ;
			case StateHeaderValue: parseStateHeaderValue(request, c);
				break ;
			case StateEndRequest: parseStateEndRequest(request, c);
				break ;
			case StateBodyStart: parseStateBodyStart(request, c);
				break ;
		}
	}
}

void HttpRequestParser::parseRequest(HttpRequest &request, const char *start, const char *end)
{
	consume(request, start, end);
}

bool HttpRequestParser::isChar(char c)
{
	return (std::isalpha(c));
}

bool HttpRequestParser::isDigit(char c)
{
	return (std::isdigit(c));
}

bool HttpRequestParser::isControlChar(char c)
{
	return (c >= 0 && c <= 31) || (c == 127);
}

bool HttpRequestParser::isReserverChar(char c)
{
	static const std::string specialChars = ":/?#[]@!$&'()*+,;={} \t";

	return (specialChars.find(c) != std::string::npos);
}

bool HttpRequestParser::isSpace(char c)
{
	return (c == ' ' || c == '\t');
}

void HttpRequestParser::parseRequestMethodStart(HttpRequest &request, char c)
{
	if (!isChar(c) || isControlChar(c) || isReserverChar(c))
		HttpRequestParser::_result = ParserError;
	else
	{
		HttpRequestParser::_state = StateRequestMethod;
		request.pushToRequestMethod(c);
	}
}

void HttpRequestParser::parseRequestMethod(HttpRequest &request, char c)
{
	if (isSpace(c))
		HttpRequestParser::_state = StateRequestURIStart;
	else if (!isChar(c) || isControlChar(c) || isReserverChar(c))
		HttpRequestParser::_result = ParserError;
	else
	{
		HttpRequestParser::_state = StateRequestMethod;
		request.pushToRequestMethod(c);
	}
}

void HttpRequestParser::parseRequestUriStart(HttpRequest &request, char c)
{
	if (isControlChar(c))
		HttpRequestParser::_result = ParserError;
	else
	{
		HttpRequestParser::_state = StateRequestURI;
		request.pushToRequestUri(c);
	}
}

void HttpRequestParser::parseRequestUri(HttpRequest &request, char c)
{
	if (isSpace(c))
		HttpRequestParser::_state = StateVersionH;
	else if (isControlChar(c))
		HttpRequestParser::_result = ParserError;
	else
		request.pushToRequestUri(c);

}

void HttpRequestParser::parseVersionH(HttpRequest &request, char c)
{
	if (c != 'H')
		HttpRequestParser::_result = ParserError;
	else
		HttpRequestParser::_state = StateVersionHT;
}

void HttpRequestParser::parseVersionHT(HttpRequest &request, char c)
{
	if (c != 'T')
		HttpRequestParser::_result = ParserError;
	else
		HttpRequestParser::_state = StateVersionHTT;
}

void HttpRequestParser::parseVersionHTT(HttpRequest &request, char c)
{
	if (c != 'T')
		HttpRequestParser::_result = ParserError;
	else
		HttpRequestParser::_state = StateVersionHTTP;
}

void HttpRequestParser::parseVersionHTTP(HttpRequest &request, char c)
{
	if (c != 'P')
		HttpRequestParser::_result = ParserError;
	else
		HttpRequestParser::_state = StateVersionHTTPSlash;
}

void HttpRequestParser::parseVersionHTTPSlash(HttpRequest &request, char c)
{
	if (c != '/')
		HttpRequestParser::_result = ParserError;
	else
		HttpRequestParser::_state = StateVersionHTTPVersionMajor;
}

void HttpRequestParser::parseVersionHTTPVersionMajor(HttpRequest &request, char c)
{
	if (!isDigit(c))
		HttpRequestParser::_result = ParserError;
	else
	{
		request.setVersionMajor(std::stoi(&c));
		HttpRequestParser::_state = StateVersionHTTPVersionDot;
	}
}

void HttpRequestParser::parseVersionHTTPVersionDot(HttpRequest &request, char c)
{
	if (c != '.')
		HttpRequestParser::_result = ParserError;
	else
		HttpRequestParser::_state = StateVersionHTTPVersionMinor;
}

void HttpRequestParser::parseVersionHTTPVersionMinor(HttpRequest &request, char c)
{
	if (!isDigit(c))
		HttpRequestParser::_result = ParserError;
	else
	{
		request.setVersionMinor(std::stoi(&c));
		HttpRequestParser::_state = StateNewLineCR;
	}
}

void HttpRequestParser::parseStateHeaderStart(HttpRequest &request, char c)
{
	if (c == '\r')
		HttpRequestParser::_state = StateEndRequest;
	else if (isControlChar(c))
		HttpRequestParser::_result = ParserError;
	else
	{
		request.pushHeader(Header());
		request.pushToLastHeaderKey(c);
		HttpRequestParser::_state = StateHeaderName;
	}
}

void HttpRequestParser::parseStateEndRequest(HttpRequest &request, char c)
{
	if (c == '\n')
		HttpRequestParser::_state = StateBodyStart;
	else
		HttpRequestParser::_result = ParserError;
}

void HttpRequestParser::parseStateHeaderName(HttpRequest &request, char c)
{
	if (isControlChar(c))
		HttpRequestParser::_result = ParserError;
	else if (c == ':')
		HttpRequestParser::_state = StateHeaderSpaceAfterName;
	else
		request.pushToLastHeaderKey(c);
}

void HttpRequestParser::parseStateSpaceAfterName(HttpRequest &request, char c)
{
	if (c != ' ')
		HttpRequestParser::_result = ParserError;
	else
		HttpRequestParser::_state = StateHeaderValue;
}

void HttpRequestParser::parseStateHeaderValue(HttpRequest &request, char c)
{
	if (c == '\r')
		HttpRequestParser::_state = StateNewLineLF;
	else if (isControlChar(c))
		HttpRequestParser::_result = ParserError;
	else
		request.pushToLastHeaderValue(c);
}


void HttpRequestParser::parseStateBodyStart(HttpRequest &request, char c)
{
	if (isSpace(c)) {}
	else if (c == '\0')
		HttpRequestParser::_result = ParserComplete;
	else
	{
		request.pushToBody(c);
	}
}


void HttpRequestParser::parseStateNewLineCR(HttpRequest &request, char c)
{
	if (c != '\r')
		HttpRequestParser::_result = ParserError;
	else
		HttpRequestParser::_state = StateNewLineLF;
}

void HttpRequestParser::parseStateNewLineLF(HttpRequest &request, char c)
{
	if (c != '\n')
		HttpRequestParser::_result = ParserError;
	else
		HttpRequestParser::_state = StateHeaderStart;
}

void HttpRequestParser::resetParser()
{
	HttpRequestParser::_state = StateRequestMethodStart;
	HttpRequestParser::_result = ParserRunning;
}
