#include "Directive.hpp"
#include <iostream>

Directive::Directive(std::string name, std::string value)
{

}

Directive::Directive()
{}

Directive::~Directive()
{}

void Directive::pushToKey(char c)
{
	this->_name.push_back(c);
}

void Directive::pushToValue(char c)
{
	this->_value.push_back(c);
}
void Directive::print()
{
	std::cout << this->_name << "\t" << this->_value << "\n";
}