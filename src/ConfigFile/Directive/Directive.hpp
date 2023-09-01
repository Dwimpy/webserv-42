#pragma once
#include <string>
#include <vector>

//typedef bool	(*checker_fct)();

class Directive {

	typedef struct s_directive_checker{
		std::string	name;
		bool	(*checkerFct)(std::string);
	}			t_directive_checker;

  public:
	Directive(std::string name, std::string value);
	Directive();
	~Directive();
	void pushToKey(char c);
	void pushToValue(char c);
	const std::string &getKey() const;
	const std::string &getValue() const;
	void print();
	const bool	isValidDirective() const;

  private:
	std::string					_name;
  	std::string					_value;

	static bool	serverNameValidKey(std::string value);
	static bool	numericalValidKey(std::string key);
	static bool errorPageValidKey(std::string value);
	static bool pageValidKey(std::string value);
	static bool limitExceptValidKey(std::string value);
	static bool	autoIndexValidKey(std::string value);
	static bool	cgiValidKey(std::string value);
};

typedef std::vector<Directive>	directiveVector;
