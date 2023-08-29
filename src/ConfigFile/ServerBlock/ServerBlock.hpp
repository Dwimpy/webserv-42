#pragma once
#include <vector>
#include <map>
#include "Directive.hpp"
#include "LocationBlock.hpp"

typedef std::vector<LocationBlock> locationBlocks;

class ServerBlock{

  public:
	ServerBlock();
	~ServerBlock();
	void	addDirective();
	void	pushToDirectiveKey(char c);
	void	pushToDirectiveValue(char c);
	void	addLocationBlock();
	void	pushToLocationPath(char c);
	void	addLocationDirective();
	void	pushToLocationDirectiveKey(char c);
	void	pushToLocationDirectiveValue(char c);
	void	printDirectives();

  private:
	directiveVector			_serverDirectives;
  	locationBlocks			_locationBlocks;
};