#pragma once
#include "ConfigFile.hpp"
#include "HttpRequest.hpp"
#include "OrderedMap.hpp"

#include <cstdint>
#include <string>

class BaseResponse {

  public:
	BaseResponse(const HttpRequest &request, const ConfigFile &config);
	virtual ~BaseResponse() = 0;
	void	setStatusCode(int16_t status_code);
	void	addHeader(const std::string &key, const std::string &value);
	void	setContentType();

	std::string	build();
  protected:
	int16_t									_status_code;
	OrderedMap<std::string, std::string>	_headers;
	std::string								_content;
	std::string								_contentType;
	const HttpRequest						&_request;
	const ConfigFile							&_config;

	void			getContent();
	std::string		getStatusCodeError() const;

	void	appendFileContents(const std::string &filename);
  private:
	int		_flag;
	int		_response_fd[2];
	void	createEnv(std::vector<std::string> &env);
	int		dup_request_to_stdin();
	void    childProcess();
	int		parent_process();
	int		write_response();
};

int		error(std::string error);
const	std::string getProjectDir();