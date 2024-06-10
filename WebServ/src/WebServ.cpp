#include "WebServ.hpp"

WebServ::WebServ(void)
{

}

WebServ::WebServ(const std::string & config)
{

}

WeServ::~WebServ(void)
{

}

void WebServ::configure(const std::string & config)
{
	std::ifstream	conf;
	std::string		contents;

	mLogger.logAccess(CONFIG_START_MSG);
	
	conf.open(config.c_str());
	if (!conf.is_open())
		throw std::runtime_error("Config file open() failed!");

	while (conf.good())
		content.push_back(conf.get());
	content.pop(); // Remove EOF
	
	if (conf.bad())
	{
		conf.close();
		throw std::runtime_error("Config file read() failed!");
	}

	conf.close();

	try { 
		this->validConfig(contents); 
	} catch (size_t row) {
		throw std::runtime_error("Config file validation failed!\nline: " + row);
	}

	this->parseConfig(contents);

	mLogger.logAccess(CONFIG_END_MSG);
}

void WebServ::activate(std::vector<std::string> envp)
{

}



void validConfig(std::string contents)
{
	size_t	pos = 0;
	size_t	rows = 0;
	int		serverAmount = 0;
	int		locationAmount = 0;
	bool	serverFlag = false;
	bool	locationFlag = false;
	bool	portFlag = false;
	bool	hostNameFlag = false;
	bool	serverNameFlag = false;
	bool	errPageFlag = false;
	bool	limitBodySizeFlag = false;
	bool	allowMethodFlag = false;
	bool	rootPathFlag = false;
	bool	indexFileFlag = false;
	bool	autoindexFlag = false;
	bool	cgiFlag = false;
	bool	uploadPathFlag = false;
	bool	redirectFlag = false;

	while (!contents.empty())
	{
		rows++;
		pos = contents.find(NEWLINE);
		if (pos == std::string::npos)
			pos = contents.size();
		
		std::vector<std::string> line = 
			ft::split(contents.substr(0, pos));
		contents.erase(0, pos);
		
		if (line.empty())
			continue;
		else if (line.front() == SERVER_BLOCK
				&& serverFlag == false
				&& locationFlag == false
				&& line.size() == 2
				&& line.back() == BLOCK_OPEN)
		{
			serverFlag = true;
			serverAmount++;
		}
		else if (line.front() == LISTEN_PORT
				&& serverFlag == true
				&& locationFlag == false
				&& portFlag == false
				&& line.size() == 2)
		{
			try {
				int port = dectoi(line.back());
				if (port < PORT_MIN || port > PORT_MAX)
					throw port;
			} catch (int e) {
				throw rows;
			}
			portFlag = true;
		}
		else if (line.front() == HOST_NAME
				&& serverFlag == true
				&& locationFlag == false
				&& hostNameFlag == false
				&& line.size() == 2)
			hostNameFlag == true;
		else if (line.front() == SERVER_NAME
				&& serverFlag == true
				&& locationFlag == false
				&& serverNameFlag == false
				&& line.size() == 2)
			serverNameFlag = true;
		else if (line.front() == DEFAULT_ERROR
				&& serverFlag == true
				&& locationFlag == false
				&& errPageFlag == false
				&& line.size() == 2)
			errPageFlag = true;
		else if (line.front() == LIMIT_BODY_SIZE
				&& serverFlag == true
				&& locationFlag == false
				&& limitBodySizeFlag == false
				&& line.size() == 2
				&& (bodySize.back() != 'M' 
					|| bodysize.back() != 'K')
		{
			std::string bodySize = line.back();
			for (size_t i = 1; i < bodySize.size() - 1; i++)
			{
				if (bodySize[i] < '0' || bodySize[i] > '9')
					throw rows;
			}
			limitBodySizeFlag = true;
		}
		else if (line.front() == LOCATION_BLOCK
				&& serverFlag == true
				&& locationFlag == false
				&& line.size() == 3
				&& line.back() == BLOCK_OPEN)
		{
			locationAmount++;
			locationFlag = true;
		}
		else if (line.front() == ALLOW_METHODS
				&& locationFlag == true
				&& allowMethodFlag == false)
		{
			for (size_t i = 1; i < line.size(); i++)
			{
				std::string method = line[i];
				if (method != "GET"
						&& method != "POST"
						&& method != "DELETE")
					throw rows;
			}
			allowMethoFlag = true;
		}
		else if (line.front() == ROOT_PATH
				&& locationFlag == true
				&& rootPathFlag == false
				&& line.size() == 2)
			rootPathFlag = true;
		else if (line.front() == INDEX_FILE
				&& locationFlag == true
				&& indexFileFlag == false
				&& line.size() == 2)
			indexFileFlag = true;
		else if (line.front() == AUTOINDEX
				&& locationFlag == true
				&& autoindexFlag == false
				&& line.size() == 2
				&& (line.back() == AUTOINDEX_ON
					|| line.back() == AUTOINDEX_OFF))
			autoindexFlag = true;
		else if (line.front() == CGI_CONFIG
				&& locationFlag == true
				&& cgiFlag == false
				&& line.size() == 3)
			cgiFlag = true;
		else if (line.front() == UPLOAD_PATH
				&& locationFlag == true
				&& uploadPathFlag == false
				&& line.size() == 2)
			uploadPathFlag = true;
		else if (line.front() == REDIRECTION
				&& locationFlag == true
				&& redirectFlag == false
				&& line.size() == 2)
			redirectFlag = true;
		else if (line.front() == BLOCK_CLOSE
				&& serverFlag == true
				&& locationFlag == false
				&& line.size() == 1
				&& portFlag == true
				&& hostNameFlag == true
				&& locationAmount > 0)
		{
			serverFlag = false;
			portFlag = false;
			hostNameFlag = false;
			serverNameFlag = false;
			errPageFlag = false;
			locationAmount = 0;
		}
		else if (line.front() == BLOCK_CLOSE
				&& locationFlag == true
				&& line.size() == 1
				&& rootPathFlag == true
				&& (indexFileFlag == true
					|| autoindexFlag == true))
		{
			allowMethodsFlag = false;
			rootPathFlag = false;
			indexFileFlag = false;
			autoindexFlag = false;
			cgiFlag = false;
			uploadPathFlag = false;
			redirectFlag = false;
			locationFlag = false;
		}
		else
			throw rows;
	}
	if (serverAmount == 0)
		throw rows;
}

void	WebServ::parseConfig(std::string & contents)
{

}
