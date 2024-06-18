#include "Connection.hpp"

Connection::Connection(void)
{
	this->mSocket = -1;
	this->mServerPort = -1;
	this->mServer = nullptr;
	this->mValidStatus = READY;
	gettimeofday(&(this->mTime), nullptr);
}

Connection::Connection(int socket, int svr_port)
{
	this->mSocket = socket;
	this->mServerPort = svr_port;
	this->mServer = nullptr;
	this->mValidStatus = READY;
	gettimeofday(&(this->mTime), nullptr);
}

Connection::~Connection(void)
{

}

void Connection::renewTime(void)
{
	gettimeofday(&this->mTime, nullptr);
}

Server *Connection::getServer(void)
{
	return this->mServer;
}

std::string Connection::getHost(void)
{
	return this->mRequest.findHeader("Host");
}

void Connection::setServer(Server *svr)
{
	if (svr == nullptr)
		return ;
	this->mServer = svr;
}

void Connection::readRequest(void)
{
	this->renewTime();
	char buffer[BUFFER_SIZE];
	int length = read(this->mSocket, buffer, BUFFER_SIZE);

  	if (length <= 0)
	{
    	this->mReqeust.set(this->mRemainStr);
	    return;
  	}
	std::string ReadStr = this->mRemainStr + buffer;

	std::string crlf = CRLF;
	size_t pos = ReadStr.find(crlf);
	while (pos != std::string::npos)
	{
		std::string line = mReadStr.substr(0, pos);
		Request.set(line);
		ReadStr.erase(0, pos + crlf.length());
		pos = ReadStr.find(crlf);
	}
	this->mRemainStr = ReadStr;
}

void Connection::validRequest(void)
{

}

void Connection::writeResponse(void)
{

}

void Connection::close(void)
{
	if (this->mSocket != -1)
		close(this->mSocket);
}

void Connection::access(void)
{

}

bool Connection::checkCompelte(void)
{

}

bool Connection::checkOvertime(void)
{
	struct timeval now;
	gettimeofday(&now, nullptr);
	if (now.tv_sec - this->mTime.tv_sec > OVERTIME)
		return true;
	return false;
}
