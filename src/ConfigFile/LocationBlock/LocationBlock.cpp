#include "LocationBlock.hpp"
#include "Directive.hpp"
#include <iostream>

LocationBlock::LocationBlock()
{}

LocationBlock::~LocationBlock()
{}

void LocationBlock::pushToPath(char c)
{
	this->_path.push_back(c);
}

void LocationBlock::addDirective()
{
	this->_directives.push_back(Directive());
}

void LocationBlock::pushToDirectiveKey(char c)
{
	this->_directives.back().pushToKey(c);
}

void LocationBlock::pushToDirectiveValue(char c)
{
	this->_directives.back().pushToValue(c);
}

void LocationBlock::print()
{
	std::cout << "Location: " << std::endl;
	for (size_t i = 0; i < _directives.size(); i++)
		_directives[i].print();
}
const std::string &LocationBlock::getPath() const
{
	return (this->_path);
}
const directiveVector &LocationBlock::getDirectives() const
{
	return (this->_directives);
}
