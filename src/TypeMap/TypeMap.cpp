#include "TypeMap.hpp"

#include <iostream>

std::map<std::string, std::string>	TypeMap::_type_map;

TypeMap::TypeMap() {
	_type_map["html"] = "text/html";
	_type_map["css"] = "text/css";
	_type_map["js"] = "application/javascript";
	_type_map["jpg"] = "image/jpeg";
	_type_map["jpeg"] = "image/jpeg";
	_type_map["png"] = "image/png";
	_type_map["gif"] = "image/gif";
	_type_map["pdf"] = "application/pdf";
	_type_map["txt"] = "text/plain";
	_type_map["json"] = "application/json";
	_type_map["xml"] = "application/xml";
	_type_map["zip"] = "application/zip";
	_type_map["tar"] = "application/x-tar";
	_type_map["mp3"] = "audio/mpeg";
	_type_map["mp4"] = "video/mp4";
	_type_map["mp3"] = "audio/mpeg";
	_type_map["svg"] = "image/svg+xml";
	_type_map["csv"] = "text/csv";
	_type_map["doc"] = "application/msword";
	_type_map["docx"] = "application/msword";
	_type_map["xls"] = "application/vnd.ms-excel";
	_type_map["xlsx"] = "application/vnd.ms-excel";
	_type_map["ppt"] = "application/vnd.ms-powerpoint";
	_type_map["pptx"] = "application/vnd.ms-powerpoint";
	_type_map["rs"] = "text/html";
	_type_map["py"] = "text/html";
	_type_map["bmp"] = "image/bmp";
	_type_map["mpg"] = "video/mpeg";
	_type_map["ico"] = "image/x-icon";
	_type_map["tiff"] = "image/tiff";
	_type_map["yaml"] = "application/x-yaml";

}
TypeMap::~TypeMap() {}


std::string TypeMap::getContentType(const std::string &extension)
{
	std::string	type;
	std::map<std::string, std::string>::iterator it;

	it = _type_map.find(extension);
	if (it != _type_map.cend())
	{
		type = it->second;
	}
	else
		type = "*/*";
	return (type);
}
