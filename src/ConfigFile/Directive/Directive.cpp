#include "Directive.hpp"
#include <iostream>
#include <sstream>

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

const std::string &Directive::getKey() const
{
	return (this->_name);
}

const std::string &Directive::getValue() const
{
	return (this->_value);
}

const bool Directive::isValidDirective() const
{
	size_t				size;
	const std::string	*ptr;
	size_t				idx;

	const std::string validValues[] = {
		"listen",
		"server_name",
		"error_page",
		"root",
		"limit_except",
		"index",
		"autoindex",
		"cgi",
		"auth",
		"client_max_body_size",
		"upload",
		"cgi_bin"
	};

	const t_directive_checker isValidValue[] = {
		{"listen", numericalValidKey},
		{"server_name", serverNameValidKey},
		{"error_page", errorPageValidKey},
		{"root", pageValidKey},
		{"allowed_methods", limitExceptValidKey},
		{"index", pageValidKey},
		{"autoindex", autoIndexValidKey},
		{"cgi", cgiValidKey},
		{"cgi_path", cgiValidKey},
		{"cgi_bin", cgiValidKey},
		{"client_max_body_size", numericalValidKey},
		{"upload", pageValidKey}

	};

	size = sizeof(isValidValue) / sizeof(isValidValue[0]);

	for (idx = 0; idx < size; ++idx)
	{
		if (isValidValue[idx].name == this->_name)
		{
			if (this->_value.empty())
				return (false);
			return (isValidValue[idx].checkerFct(this->_value));
		}
	}
	return (false);
}

bool	Directive::numericalValidKey(std::string value)
{
	try {
		size_t	pos;
		std::stoi(value, &pos);
		if (pos != value.length())
			throw std::runtime_error("Value is not numerical");
	}catch (std::exception &e){
		std::cerr << e.what() << std::endl;
		return (false);
	}
	return (true);
}

bool	Directive::serverNameValidKey(std::string value)
{
	for (size_t i = 0; i < value.length(); ++i)
	{
		if (!std::isalnum(value[i]) && value[i] != '-' && value[i] != '_' && value[i] != '.')
			return (false);
	}
	return (true);
}

bool	Directive::errorPageValidKey(std::string value)
{
	std::vector<std::string>	tokens;
	std::istringstream			istream(value);
	std::string					token;

	while (istream >> token)
		tokens.push_back(token);
	if (tokens.size() != 2)
		return (false);

	try{
		std::stoi(tokens[0]);
		for (size_t i = 0; i < tokens[1].length(); ++i)
		{
			if (!std::isalnum(tokens[1][i]) && tokens[1][i] != '-' && tokens[1][i] != '_' && tokens[1][i] != '/' && tokens[1][i] != '.')
				throw std::runtime_error("Invalid directive value");
		}
		return (true);
	}catch (std::exception &e){
		std::cerr << e.what() << std::endl;
		return (false);
	}
}

bool	Directive::pageValidKey(std::string value)
{
	for (size_t i = 0; i < value.length(); ++i)
	{
		if (!std::isalnum(value[i]) && value[i] != '-' && value[i] != '_' && value[i] != '.' && value[i] != '/')
			return (false);
	}
	return (true);
}

bool	Directive::limitExceptValidKey(std::string value)
{
	std::vector<std::string>	tokens;
	std::istringstream			istream(value);
	std::string					token;

	while (istream >> token)
		tokens.push_back(token);
	for (std::vector<std::string>::const_iterator it = tokens.cbegin(); it != tokens.cend(); ++it)
	{
		if (*it != "GET" && *it != "POST" && *it != "DELETE" && *it != "HEAD" && *it != "PUT")
			return (false);
	}
	return (true);
}

bool	Directive::autoIndexValidKey(std::string value)
{
	return (value == "on" || value == "off");
}

bool	Directive::cgiValidKey(std::string value)
{
	return (true);
}

