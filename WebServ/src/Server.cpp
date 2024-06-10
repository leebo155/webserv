#include "Server.hpp"

Server::Server(void)
{
	this->mPort = -1;
	this->mSocket = -1;
	this->mBodySize = 4096;
	this->mServerName = DEFAULT_SERVERNAME;
}

Server::~Server(void)
{
	this->mLocation.clear();
}

int	Server::getPort(void)
{
	return this->mPort;
}

int Server::getSocket(void)
{
	return this->mSocket;
}

Location *Server::getLocation(size_t idx)
{
	if (this->mLocation.empty
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

void Server::setBodySize(std::string & dataSize)
{
	char sizeForm = dataSize.back();
	dataSize.pop();
	size_t bodysize = dectoi(dataSize);
	switch (sizeForm) {
		case 'M':
			bodysize *= 1000000;
			break;
		case 'K':
			bodysize *= 1000;
	}
	this->mBodySize = bodysize;
}

void Server::setHost(std::string & host)
{

}

void Server::setServerName(std::string & servername)
{

}

void Server::setErrorPage(std::string & file)
{

}

void Server::addLocation(Location & lct)
{

}
