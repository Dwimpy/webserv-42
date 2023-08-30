#pragma once
#include "ServerConfigurator.hpp"

class ConfigFileParser{

	enum ParserResult{
		ParserError,
		ParserWaiting,
		ParserRunning,
		ParserComplete
	};

	enum ParserState {
		StateInServerBlock,
		StateInDirectiveKey,
		StateInDirectiveValue,
		StateInDirectiveSemiColon,
		StateFindingServerBlockS,
		StateFindingServerBlockSE,
		StateFindingServerBlockSER,
		StateFindingServerBlockSERV,
		StateFindingServerBlockSERVE,
		StateFindingServerBlockSERVER,
		StateFindingServerBlockSpace,
		StateFindingServerOpenBracket,
		StateInFindingLocationBlockLO,
		StateInFindingLocationBlockLOC,
		StateInFindingLocationBlockLOCA,
		StateInFindingLocationBlockLOCAT,
		StateInFindingLocationBlockLOCATI,
		StateInFindingLocationBlockLOCATIO,
		StateInFindingLocationBlockLOCATION,
		StateInFindingLocationBlockSpace,
		StateInFindingLocationBlockPath,
		StateInFindingLocationBlockOpenBracket,
		StateInLocationBlock,
		StateInLocationDirectiveKey,
		StateInLocationDirectiveValue,
	};

  public:
	static void	parseFile(ServerConfigurator &configurator, const char *start, const char *end);
	static void	consume(ServerConfigurator &configurator, const char *start, const char *end);
	static void resetParser();
	static bool isParsingError();
  private:

	static void parseStateFindingServerBlockS(ServerConfigurator &configurator, char c);
	static void parseStateFindingServerBlockSE(ServerConfigurator &configurator, char c);
	static void parseStateFindingServerBlockSER(ServerConfigurator &configurator, char c);
	static void parseStateFindingServerBlockSERV(ServerConfigurator &configurator, char c);
	static void parseStateFindingServerBlockSERVE(ServerConfigurator &configurator, char c);
	static void parseStateFindingServerBlockSERVER(ServerConfigurator &configurator, char c);
	static void parseStateFindingServerBlockSpace(ServerConfigurator &configurator, char c);
	static void parseStateFindingServerBlockOpenBracket(ServerConfigurator &configurator, char c);
	static void parseStateInServerBlock(ServerConfigurator &configurator, const std::string &word,  char c);
	static void parseStateInDirectiveKey(ServerConfigurator &configurator, char c);
	static void parseStateInDirectiveValue(ServerConfigurator &configurator, char c);
//	static void parseStateInLocationBlock(ServerConfigurator &configurator, char c);
	static void parseStateFindingLocationBlockLo(ServerConfigurator &configurator, char c);
	static void parseStateFindingLocationBlockLoc(ServerConfigurator &configurator, char c);
	static void parseStateFindingLocationBlockLoca(ServerConfigurator &configurator, char c);
	static void parseStateFindingLocationBlockLocat(ServerConfigurator &configurator, char c);
	static void parseStateFindingLocationBlockLocati(ServerConfigurator &configurator, char c);
	static void parseStateFindingLocationBlockLocatio(ServerConfigurator &configurator, char c);
	static void parseStateFindingLocationBlockLocation(ServerConfigurator &configurator, char c);
	static void parseStateFindingLocationBlockSpace(ServerConfigurator &configurator, char c);
	static void parseStateFindingLocationPath(ServerConfigurator &configurator, char c);
	static void parseStateFindingLocationOpenBracket(ServerConfigurator &configurator, char c);
	static void parseStateLocationBlock(ServerConfigurator &configurator, char c);
	static void parseStateLocationDirectiveKey(ServerConfigurator &configurator, char c);
	static void parseStateLocationDirectiveValue(ServerConfigurator &configurator, char c);

	static bool isChar(char c);
	static bool isDigit(char c);
	static bool isSpace(char c);
	static bool isControlChar(char c);
	static bool isAllowedKeyChar(char c);
	static bool isAllowedValueChar(char c);
	static bool isAllowedLocationPathChar(char c);
	static bool isReserverChar(char c);


	static ParserResult		_result;
	static ParserState		_state;

};