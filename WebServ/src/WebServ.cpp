#include "WebServ.hpp"

void WebServ::printAll(void)
{
	for (std::vector<Server>::iterator it = this->mServers.begin(); it != this->mServers.end(); it++)
	{
		std::cout << "Server {" << std::endl;
		it->printAll();
		std::cout << "}" << std::endl;
	}
}

WebServ::WebServ(void)
{
}

WebServ::WebServ(const std::string & config)
{
	this->configure(config);
}

WebServ::~WebServ(void)
{
	mServers.clear();
}

void WebServ::configure(const std::string & config)
{
	std::ifstream	conf;
	std::string		contents;

//	mLogger.logAccess(CONFIG_START_MSG);
	std::cout << "start configure" << std::endl;
	
	conf.open(config.c_str());
	if (!conf.is_open())
		throw std::runtime_error("Config file open() failed!");

	while (conf.good())
		contents.push_back(conf.get());
	contents.pop_back(); // Remove EOF
	
	if (conf.bad())
	{
		conf.close();
		throw std::runtime_error("Config file read() failed!");
	}

	conf.close();

	try { 
		this->validConfig(contents); 
	} catch (size_t row) {
		throw std::runtime_error("Config file validation failed! line: " + ft::toString(row, 10));
	}

	this->parseConfig(contents);

//	mLogger.logAccess(CONFIG_END_MSG);
	std::cout << "end configure" << std::endl;
}

void WebServ::activate(char *envp[])
{
	std::cout << "========= start activate() =========" << std::endl;
	this->printAll();
	(void)envp;
	std::cout << "========= end activate() =========" << std::endl;
}



void WebServ::validConfig(std::string contents)
{
	std::cout << "========= start validConfig() =========" << std::endl;
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
	bool	uploadPathFlag = false;
	bool	redirectFlag = false;

	while (!contents.empty())
	{
		rows++;
		pos = contents.find(NEWLINE);
		if (pos == std::string::npos)
			pos = contents.size();
		
		std::cout << "line: " << contents.substr(0, pos) << std::endl;
		std::vector<std::string> line = 
			ft::split(contents.substr(0, pos));
		contents.erase(0, pos + 1);
		
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
				int port = ft::toInt(line.back(), 10);
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
			hostNameFlag = true;
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
				&& (line.back().back() != 'M' 
					|| line.back().back() != 'K'))
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
			allowMethodFlag = true;
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
		{
			if (line.back() == AUTOINDEX_ON)
				autoindexFlag = true;
		}
		else if (line.front() == CGI_CONFIG
				&& locationFlag == true
				&& line.size() == 3)
			continue;
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
			allowMethodFlag = false;
			rootPathFlag = false;
			indexFileFlag = false;
			autoindexFlag = false;
			uploadPathFlag = false;
			redirectFlag = false;
			locationFlag = false;
		}
		else
			throw rows;
	}
	if (serverAmount == 0)
		throw rows;
	std::cout << "========= end validConfig() =========" << std::endl;
}

void	WebServ::parseConfig(std::string & contents)
{
	std::cout << "========= start parseConfig() =========" << std::endl;
	size_t		pos = 0;
	Location	*cur = NULL;
	size_t		locationIdx = 0;
	bool		locationCloseFlag = false;

	while (!contents.empty())
	{
		pos = contents.find(NEWLINE);
		if (pos == std::string::npos)
			pos = contents.size();
		
		std::cout << "line: " << contents.substr(0, pos) << std::endl;
		std::vector<std::string> line = 
			ft::split(contents.substr(0, pos));
		contents.erase(0, pos + 1);
		if (line.empty())
			continue;
		else if (line.front() == SERVER_BLOCK)
			this->mServers.push_back(Server());
		else if (line.front() == LISTEN_PORT)
			this->mServers.back().setPort(ft::toInt(line.back(), 10));
		else if (line.front() == HOST_NAME)
			this->mServers.back().setHost(line.back());
		else if (line.front() == SERVER_NAME)
			this->mServers.back().setServerName(line.back());
		else if (line.front() == DEFAULT_ERROR)
			this->mServers.back().setErrorPage(line.back(), "");
		else if (line.front() == LIMIT_BODY_SIZE)
			this->mServers.back().setBodySize(line.back());
		else if (line.front() == LOCATION_BLOCK)
		{
			locationCloseFlag = true;
			this->mServers.back().addEmptyLocation();
			cur = this->mServers.back().getLocation(locationIdx);
			cur->setPath(line[1]);
		}
		else if (line.front() == ALLOW_METHODS)
		{
			cur->clearMethod();
			for (size_t i = 1; i < line.size(); i++)
				cur->addMethod(line[i]);
		}
		else if (line.front() == ROOT_PATH)
			cur->setRoot(line.back());
		else if (line.front() == INDEX_FILE)
			cur->setIndex(line.back());
		else if (line.front() == AUTOINDEX)
		{
			if (line.back() == AUTOINDEX_ON)
				cur->setAutoindex(true);
			else if (line.back() == AUTOINDEX_OFF)
				cur->setAutoindex(false);
		}
		else if (line.front() == CGI_CONFIG)
			cur->addCgi(line[1], line[2]);
		else if (line.front() == UPLOAD_PATH)
			cur->setUpload(line.back());
		else if (line.front() == REDIRECTION)
			cur->setRedirect(line.back());
		else if (line.front() == BLOCK_CLOSE
				&& locationCloseFlag == true)
		{
			locationIdx++;
			locationCloseFlag = false;
		}
		else if (line.front() == BLOCK_CLOSE
				&& locationCloseFlag == false)
			locationIdx = 0;
	}
	std::cout << "========= end parseConfig() =========" << std::endl;
}
