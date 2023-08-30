#pragma once
#include <vector>
#include <map>
#include "Directive.hpp"


class LocationBlock {

  public:
	LocationBlock();
	~LocationBlock();
	void pushToPath(char c);
	void print();
	void addDirective();
	void pushToDirectiveKey(char c);
	void pushToDirectiveValue(char c);
	const std::string &getPath() const;
	const directiveVector	&getDirectives() const;

  private:
	std::string		_path;
	directiveVector	_directives;
};