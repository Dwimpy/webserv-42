#pragma once
#include <string>
#include <vector>


class Directive {
  public:
	Directive(std::string name, std::string value);
	Directive();
	~Directive();
	void pushToKey(char c);
	void pushToValue(char c);
	void print();
  private:
	std::string _name;
  	std::string _value;
};

typedef std::vector<Directive>	directiveVector;
