#include "ServerBlock.hpp"
#include "Directive.hpp"
#include "LocationBlock.hpp"
#include "Server.hpp"

ServerBlock::ServerBlock()
{}

ServerBlock::~ServerBlock()
{}

void ServerBlock::addDirective()
{
	this->_serverDirectives.push_back(Directive());
}

void ServerBlock::pushToDirectiveKey(char c)
{
	this->_serverDirectives.back().pushToKey(c);
}

void ServerBlock::pushToDirectiveValue(char c)
{
	this->_serverDirectives.back().pushToValue(c);
}

void ServerBlock::addLocationBlock()
{
	this->_locationBlocks.__emplace_back(LocationBlock());
}

void ServerBlock::pushToLocationPath(char c)
{
	this->_locationBlocks.back().pushToPath(c);
}

void ServerBlock::printDirectives()
{
	for (size_t i = 0; i < _serverDirectives.size(); i++)
		_serverDirectives[i].print();

	for (size_t i = 0; i < _locationBlocks.size(); i++)
		_locationBlocks[i].print();
}
void ServerBlock::addLocationDirective()
{
	this->_locationBlocks.back().addDirective();
}

void ServerBlock::pushToLocationDirectiveKey(char c)
{
	this->_locationBlocks.back().pushToDirectiveKey(c);
}

void ServerBlock::pushToLocationDirectiveValue(char c)
{
	this->_locationBlocks.back().pushToDirectiveValue(c);
}

const directiveVector &ServerBlock::getServerDirectives() const
{
	return (this->_serverDirectives);
}

const locationBlocks &ServerBlock::getLocationBlocks() const
{
	return (this->_locationBlocks);
}
