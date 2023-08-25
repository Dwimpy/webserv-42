#include "HttpRequestParser.hpp"

HttpRequestParser::ParserState HttpRequestParser::_state;

void	HttpRequestParser::consume(HttpRequest &request, const char *start, const char *end)
{
	while (start != end)
	{
		char c = *start++;
		switch(_state)
		{
			case StateRequestMethodStart:
				request.pushToRequestMethod(c);
				break ;
			case StateRequestMethod:

				break ;
		}
	}
}

void	HttpRequestParser::parseRequest(HttpRequest &request, const char *start, const char *end)
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
