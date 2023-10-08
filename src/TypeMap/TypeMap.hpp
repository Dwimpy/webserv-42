#include <string>
#include <map>

class TypeMap {

  public:
	TypeMap();
	~TypeMap();
	static std::string getContentType(const std::string &extension);

  private:
	static std::map<std::string, std::string>	_type_map;
};