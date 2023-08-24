#pragma once

# include <string>

class ServerConfig {
  public:
	// Default constructors
	ServerConfig();
	ServerConfig(const std::string &filename);
	~ServerConfig();
	ServerConfig &operator=(const ServerConfig &copy);

	// Setters
	void			setPort(const std::string& port);
	void			setHostName(const std::string& hostname);
	void			setHost(const std::string& host);
	void			setDocumentRoot(const std::string& root);
	void			setIndex(const std::string& index);

	// Getters
	const std::string	&getPort() const;
	const std::string	&getHostName() const;
	const std::string	&getHost() const;
	const std::string	&getDocumentRoot() const;
	const std::string	&getIndex() const;

  private:
	std::string		_port;
	std::string		_hostname;
	std::string		_host;
	std::string		_documentRoot;
	std::string		_index;
};
