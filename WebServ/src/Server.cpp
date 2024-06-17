#include "Server.hpp"

Server::Server(void)
{
	this->mPort = -1;
	this->mSocket = -1;
	this->mServerName = DEFAULT_SERVERNAME;
	this->setBodySize(DEFAULT_BODY_SIZE);

}

Server::~Server(void)
{
	this->mLocation.clear();
}

void Server::printAll(void)
{
	std::cout << "\tPort: " << this->mPort << std::endl; 
	std::cout << "\tSocket: " << this->mSocket << std::endl; 
	std::cout << "\tBody Size: " << this->mBodySize << std::endl; 
	std::cout << "\tHost: " << this->mHost << std::endl; 
	std::cout << "\tServer Name: " << this->mServerName << std::endl; 
	std::cout << "\tError Page {" << std::endl;
	std::cout << "\t\tStatus: " << this->mErrorPage.getStatus() << std::endl;
	std::cout << "\t\tCode: " << this->mErrorPage.getCode() << std::endl;
	std::cout << "\t\tMsg: " << this->mErrorPage.getStatusMsg() << std::endl;
	std::cout << "\t\tContent-Type: " << this->mErrorPage.getContentType() << std::endl;
	std::cout << "\t\tContent-Length: " << this->mErrorPage.getContentLength() << std::endl;
	std::cout << "\t\tRedirection: " << this->mErrorPage.getRedirection() << std::endl;
	std::cout << "\t\tServerName: " << this->mErrorPage.getServerName() << std::endl;
	std::cout << "\t\tBody {\n\t\t\t" << this->mErrorPage.getBody() << "\n\t\t}" << std::endl;
	for (std::vector<Location>::iterator it = this->mLocation.begin(); it != this->mLocation.end(); it++)
	{
		std::cout << "\tLocation " << it->getPath() << " {" << std::endl;
		it->printAll();
		std::cout << "\t}" << std::endl;
	}

}

int	Server::getPort(void)
{
	return this->mPort;
}

int Server::getSocket(void)
{
	return this->mSocket;
}

std::string Server::getHost(void)
{
	return this->mHost;
}

std::string Server::getServerName(void)
{
	return this->mServerName;
}

Location *Server::getLocation(size_t idx)
{
	if (this->mLocation.empty()
			|| idx < 0
			|| this->mLocation.size() < idx)
		return NULL;
	return &this->mLocation[idx];
}

size_t	Server::getLocationSize(void)
{
	return this->mLocation.size();
}

void Server::setPort(int port)
{
	this->mPort = port;
}

void Server::setSocket(int socket)
{
	this->mSocket = socket;
}

void Server::setBodySize(std::string const & dataSize)
{
	std::string tmp = dataSize;
	char sizeForm = tmp.back();
	tmp.pop_back();
	size_t bodysize = ft::toInt(tmp, 10);
	switch (sizeForm) {
		case 'M':
			bodysize *= 1000000;
			break;
		case 'K':
			bodysize *= 1000;
	}
	this->mBodySize = bodysize;
}

void Server::setHost(std::string const & host)
{
	this->mHost = host;
}

void Server::setServerName(std::string const & servername)
{
	this->mServerName = servername;
}

void Server::setErrorPage(std::string const & file, std::string const & type)
{
	this->mErrorPage = file;
	this->mErrorPageType = type;
}

Response Server::getErrorPage(int errcode, std::string const & errmsg)
{
	std::string body;
	Response errorPage;
	errorPage.setServerName(this->mServerName);

	try {
		if (this->mErrorPage.empty())
			throw ;
		std::ifstream errPage;
		errPage.open(this->mErrorPage.c_str());
		if (!errPage.is_open())
			throw ;
		while (errPage.good())
			body.push_back(errPage.get());
		body.pop_back();
		if (errPage.bad())
		{
			errPage.close();
			throw ;
		}
		errPage.close();
		errorPage.setContentType(this->mErrorPageType);
	} catch (...) {
		body = this->getDefaultError(errcode, errmsg);
		errorPage.setContentType("text/html");
	}

	errorPage.setBody(body);
	return errorPage;
}

void Server::addEmptyLocation(void)
{
	this->mLocation.push_back(Location());
}

std::string Server::getDefaultError(int errcode, std::string const & errmsg)
{
	std::string error_code = ft::toString(errcode);
	std::string body = "<!DOCTYPE html>";
	body += "\n<html lang=\"en\">";
	body += "\n<head>";
	body += "\n\t<meta charset=\"UTF-8\">";
	body += "\n\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
	body += "\n\t<title>";
	body += error_code;
	body += " ";
	body += errmsg;
	body += "</title>";
	body += "\n\t<style>";
	body += "\n\t\tbody {";
	body += "\n\t\t\tbackground-color: #f8f8f8;";
	body += "\n\t\t\tcolor: #333;";
	body += "\n\t\t\tfont-family: Arial, sans-serif;";
	body += "\n\t\t\ttext-align: center;";
	body += "\n\t\t}";
	body += "\n\t\th1 {";
	body += "\n\t\t\tfont-size: 50px;";
	body += "\n\t\t}";
	body += "\n\t\tp {";
	body += "\n\t\t\tfont-size: 20px;";
	body += "\n\t\t}";
	body += "\n\t</style>";
	body += "\n</head>";
	body += "\n<body>";
	body += "\n\t<h1>";
	body += error_code;
	body += " ";
	body += errmsg;
	body += "</h1>";
	body += "\n\t<hr>";
	body += "\n\t<p>";
	body += this->mServerName;
	body += "</p>";
	body += "\n</body>";
	body += "\n</html>";

	return body;
}
