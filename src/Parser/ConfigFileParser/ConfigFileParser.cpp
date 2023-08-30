#include "ConfigFileParser.hpp"
#include <cctype>

ConfigFileParser::ParserState ConfigFileParser::_state = StateFindingServerBlockS;
ConfigFileParser::ParserResult ConfigFileParser::_result = ParserWaiting;

void ConfigFileParser::consume(ServerConfigurator &configurator, const char *start, const char *end)
{
	while (start != end && ConfigFileParser::_result != ParserError)
	{
		char c = *start++;
		switch (ConfigFileParser::_state)
		{
			case StateFindingServerBlockS: parseStateFindingServerBlockS(configurator, c);
				break ;
			case StateFindingServerBlockSE: parseStateFindingServerBlockSE(configurator, c);
				break ;
			case StateFindingServerBlockSER: parseStateFindingServerBlockSER(configurator, c);
				break ;
			case StateFindingServerBlockSERV: parseStateFindingServerBlockSERV(configurator, c);
				break ;
			case StateFindingServerBlockSERVE: parseStateFindingServerBlockSERVE(configurator, c);
				break ;
			case StateFindingServerBlockSERVER: parseStateFindingServerBlockSERVER(configurator, c);
				break ;
			case StateFindingServerBlockSpace: parseStateFindingServerBlockSpace(configurator, c);
				break ;
			case StateFindingServerOpenBracket: parseStateFindingServerBlockOpenBracket(configurator, c);
				break ;
			case StateInServerBlock: parseStateInServerBlock(configurator, std::string(start - 1), c);
				break ;
			case StateInDirectiveKey: parseStateInDirectiveKey(configurator, c);
				break ;
			case StateInDirectiveValue: parseStateInDirectiveValue(configurator, c);
				break ;
			case StateInFindingLocationBlockLO: parseStateFindingLocationBlockLo(configurator, c);
				break ;
			case StateInFindingLocationBlockLOC: parseStateFindingLocationBlockLoc(configurator, c);
				break ;
			case StateInFindingLocationBlockLOCA: parseStateFindingLocationBlockLoca(configurator, c);
				break ;
			case StateInFindingLocationBlockLOCAT: parseStateFindingLocationBlockLocat(configurator, c);
				break ;
			case StateInFindingLocationBlockLOCATI: parseStateFindingLocationBlockLocati(configurator, c);
				break ;
			case StateInFindingLocationBlockLOCATIO: parseStateFindingLocationBlockLocatio(configurator, c);
				break ;
			case StateInFindingLocationBlockLOCATION: parseStateFindingLocationBlockLocation(configurator, c);
				break ;
			case StateInFindingLocationBlockSpace: parseStateFindingLocationBlockSpace(configurator, c);
				break ;
			case StateInFindingLocationBlockPath: parseStateFindingLocationPath(configurator, c);
				break ;
			case StateInFindingLocationBlockOpenBracket: parseStateFindingLocationOpenBracket(configurator, c);
				break ;
			case StateInLocationBlock: parseStateLocationBlock(configurator, c);
				break ;
			case StateInLocationDirectiveKey: parseStateLocationDirectiveKey(configurator, c);
				break ;
			case StateInLocationDirectiveValue: parseStateLocationDirectiveValue(configurator, c);
				break ;
			default: break;
		}
	}
}

void ConfigFileParser::parseFile(ServerConfigurator &configurator, const char *start, const char *end)
{
	ConfigFileParser::_result = ParserRunning;
	consume(configurator, start, end);
	if (ConfigFileParser::_result != ParserError)
		ConfigFileParser::_result = ParserComplete;
}

bool ConfigFileParser::isParsingError()
{
	return (_result == ParserError);
}

void ConfigFileParser::resetParser()
{
	ConfigFileParser::_state = StateFindingServerBlockS;
	ConfigFileParser::_result = ParserWaiting;
}

bool ConfigFileParser::isChar(char c)
{
	return (std::isalpha(c));
}

bool ConfigFileParser::isDigit(char c)
{
	return (std::isdigit(c));
}

bool ConfigFileParser::isAllowedKeyChar(char c)
{
	return (std::isalpha(c) || std::isdigit(c) || c == '_' || c == '-');
}

bool ConfigFileParser::isAllowedValueChar(char c)
{
	return (std::isalpha(c) || std::isdigit(c) || c == '_' || c == '-' || c == '.' || c == '/' || c == ':');
}

bool ConfigFileParser::isAllowedLocationPathChar(char c)
{
	return (std::isalpha(c) || std::isdigit(c) || c == '_' || c == '-' || c == '/');
}

bool ConfigFileParser::isControlChar(char c)
{
	return (c >= 0 && c <= 31) || (c == 127);
}

bool ConfigFileParser::isReserverChar(char c)
{
	static const std::string specialChars = ":/?#[]@!$&'()*+,;={} \t";

	return (specialChars.find(c) != std::string::npos);
}

bool ConfigFileParser::isSpace(char c)
{
	return (c == ' ' || c == '\t');
}

void	ConfigFileParser::parseStateFindingServerBlockS(ServerConfigurator &configurator, char c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f')
	{}
	else if (c != 's' && c != 'S')
		ConfigFileParser::_result = ParserError;
	else
		ConfigFileParser::_state = StateFindingServerBlockSE;
}
void ConfigFileParser::parseStateFindingServerBlockSE(ServerConfigurator &configurator, char c)
{
	if (c != 'e' && c != 'E')
		ConfigFileParser::_result = ParserError;
	else
		ConfigFileParser::_state = StateFindingServerBlockSER;
}
void ConfigFileParser::parseStateFindingServerBlockSER(ServerConfigurator &configurator, char c)
{
	if (c != 'r' && c != 'R')
		ConfigFileParser::_result = ParserError;
	else
		ConfigFileParser::_state = StateFindingServerBlockSERV;
}
void ConfigFileParser::parseStateFindingServerBlockSERV(ServerConfigurator &configurator, char c)
{
	if (c != 'v' && c != 'V')
		ConfigFileParser::_result = ParserError;
	else
		ConfigFileParser::_state = StateFindingServerBlockSERVE;
}
void ConfigFileParser::parseStateFindingServerBlockSERVE(ServerConfigurator &configurator, char c)
{
	if (c != 'e' && c != 'E')
		ConfigFileParser::_result = ParserError;
	else
		ConfigFileParser::_state = StateFindingServerBlockSERVER;
}
void ConfigFileParser::parseStateFindingServerBlockSERVER(ServerConfigurator &configurator, char c)
{
	if (c != 'r' && c != 'R')
		ConfigFileParser::_result = ParserError;
	else
		ConfigFileParser::_state = StateFindingServerBlockSpace;
}

void ConfigFileParser::parseStateFindingServerBlockSpace(ServerConfigurator &configurator, char c)
{
	if (c != ' ')
		ConfigFileParser::_result = ParserError;
	else
		ConfigFileParser::_state = StateFindingServerOpenBracket;
}

void ConfigFileParser::parseStateFindingServerBlockOpenBracket(ServerConfigurator &configurator, char c)
{
	if (c != '{')
		ConfigFileParser::_result = ParserError;
	else
	{
		configurator.addEmptyServerBlock();
		ConfigFileParser::_state = StateInServerBlock;
	}
}

void ConfigFileParser::parseStateInServerBlock(ServerConfigurator &configurator, const std::string &word,  char c)
{
	if (c == '\n') {}
	else if (c == '}')
		ConfigFileParser::_state = StateFindingServerBlockS;
	else if (word.substr(0, 8) == "location")
	{
		configurator.addLocationBlock();
		ConfigFileParser::_state = StateInFindingLocationBlockLO;
	}
	else if (isAllowedKeyChar(c))
	{
		configurator.addServerDirective();
		ConfigFileParser::_state = StateInDirectiveKey;
		configurator.pushToServerDirectiveKey(c);
	}
	else
		ConfigFileParser::_result = ParserError;
}

void ConfigFileParser::parseStateInDirectiveKey(ServerConfigurator &configurator, char c)
{
	if (isSpace(c))
		ConfigFileParser::_state = StateInDirectiveValue;
	else if (!isAllowedKeyChar(c))
		ConfigFileParser::_result = ParserError;
	else
		configurator.pushToServerDirectiveKey(c);
}

void ConfigFileParser::parseStateInDirectiveValue(ServerConfigurator &configurator, char c)
{
	if (c == ';')
		ConfigFileParser::_state = StateInServerBlock;
	else if (!isAllowedValueChar(c) && !isSpace(c))
		ConfigFileParser::_result = ParserError;
	else
		configurator.pushToServerDirectiveValue(c);
}

void ConfigFileParser::parseStateFindingLocationBlockLo(ServerConfigurator &configurator, char c)
{
	if (c != 'o' && c != 'O')
		ConfigFileParser::_result = ParserError;
	else
		ConfigFileParser::_state = StateInFindingLocationBlockLOC;
}

void ConfigFileParser::parseStateFindingLocationBlockLoc(ServerConfigurator &configurator, char c)
{
	if (c != 'c'  && c != 'C')
		ConfigFileParser::_result = ParserError;
	else
		ConfigFileParser::_state = StateInFindingLocationBlockLOCA;
}

void ConfigFileParser::parseStateFindingLocationBlockLoca(ServerConfigurator &configurator, char c)
{
	if (c != 'a' && c != 'A')
		ConfigFileParser::_result = ParserError;
	else
		ConfigFileParser::_state = StateInFindingLocationBlockLOCAT;
}

void ConfigFileParser::parseStateFindingLocationBlockLocat(ServerConfigurator &configurator, char c)
{
	if (c != 't' && c != 'T')
		ConfigFileParser::_result = ParserError;
	else
		ConfigFileParser::_state = StateInFindingLocationBlockLOCATI;
}

void ConfigFileParser::parseStateFindingLocationBlockLocati(ServerConfigurator &configurator, char c)
{
	if (c != 'i' && c != 'I')
		ConfigFileParser::_result = ParserError;
	else
		ConfigFileParser::_state = StateInFindingLocationBlockLOCATIO;
}

void ConfigFileParser::parseStateFindingLocationBlockLocatio(ServerConfigurator &configurator, char c)
{
	if (c != 'o' && c != 'O')
		ConfigFileParser::_result = ParserError;
	else
		ConfigFileParser::_state = StateInFindingLocationBlockLOCATION;
}

void ConfigFileParser::parseStateFindingLocationBlockLocation(ServerConfigurator &configurator, char c)
{
	if (c != 'n' && c != 'N')
		ConfigFileParser::_result = ParserError;
	else
		ConfigFileParser::_state = StateInFindingLocationBlockSpace;
}

void ConfigFileParser::parseStateFindingLocationBlockSpace(ServerConfigurator &configurator, char c)
{
	if (!isSpace(c))
		ConfigFileParser::_result = ParserError;
	else
		ConfigFileParser::_state = StateInFindingLocationBlockPath;
}

void ConfigFileParser::parseStateFindingLocationPath(ServerConfigurator &configurator, char c)
{
	if (isSpace(c))
		ConfigFileParser::_state = StateInFindingLocationBlockOpenBracket;
	else if (!isAllowedLocationPathChar(c))
		ConfigFileParser::_result = ParserError;
	else
		configurator.pushToLocationPath(c);
}

void ConfigFileParser::parseStateFindingLocationOpenBracket(ServerConfigurator &configurator, char c)
{
	if (isSpace(c)) {}
	else if (c != '{')
		ConfigFileParser::_result = ParserError;
	else
		ConfigFileParser::_state = StateInLocationBlock;
}

void ConfigFileParser::parseStateLocationBlock(ServerConfigurator &configurator, char c)
{
	if (c == '\n') {}
	else if (c == '}')
		ConfigFileParser::_state = StateInServerBlock;
	else if (isAllowedKeyChar(c))
	{
		configurator.addLocationDirective();
		configurator.pushToLocationDirectiveKey(c);
		ConfigFileParser::_state = StateInLocationDirectiveKey;
	}
	else
		ConfigFileParser::_result = ParserError;
}

void ConfigFileParser::parseStateLocationDirectiveKey(ServerConfigurator &configurator, char c)
{
	if (isSpace(c))
		ConfigFileParser::_state = StateInLocationDirectiveValue;
	else if (!isAllowedKeyChar(c))
		ConfigFileParser::_result = ParserError;
	else
		configurator.pushToLocationDirectiveKey(c);
}

void ConfigFileParser::parseStateLocationDirectiveValue(ServerConfigurator &configurator, char c)
{
	if (c == ';')
		ConfigFileParser::_state = StateInLocationBlock;
	else if (!isAllowedValueChar(c) && !isSpace(c))
		ConfigFileParser::_result = ParserError;
	else
		configurator.pushToLocationDirectiveValue(c);
}