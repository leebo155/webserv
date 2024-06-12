#include "Location.hpp"
#include <iostream>

void Location::printAll(void)
{
	std::cout << "\t\tRoot: " << this->mRoot << std::endl;
	std::cout << "\t\tIndex: " << this->mIndex << std::endl;
	std::cout << "\t\tMethod: ";
	for (std::vector<std::string>::iterator it = this->mMethod.begin(); it != this->mMethod.end(); it++)
		std::cout << *it << " ";
	std::cout << std::endl;
	std::cout << "\t\tRedirection: " << this->mRedirect << std::endl;
	std::cout << "\t\tUpload Directory: " << this->mUpload << std::endl;
	std::cout << "\t\tAutoindex: " << (this->mAutoindex ? "true" : "false") << std::endl;
	std::cout << "\t\tCGI {" << std::endl;
	for (std::map<std::string, std::string>::iterator it = this->mCgi.begin(); it != this->mCgi.end(); it++)
		std::cout << "\t\t\t" << it->first << ": " << it->second << std::endl;
	std::cout<< "\t\t}" << std::endl;
}

Location::Location(void)
{
	this->mAutoindex = false;
	this->mMethod.push_back("GET");
	this->mMethod.push_back("POST");
	this->mMethod.push_back("DELETE");
}

Location::~Location(void)
{
	this->mMethod.clear();
	this->mCgi.clear();
}

std::string Location::getPath(void)
{
	return this->mPath;
}

std::string Location::getIndex(void)
{
	return this->mIndex;
}

std::string Location::getRoot(void)
{
	return this->mRoot;
}

std::string Location::getRedirect(void)
{
	return this->mRedirect;
}

std::string Location::getUpload(void)
{
	return this->mUpload;
}

std::string Location::getCgi(std::string & extension)
{
	return this->mCgi[extension];
}

bool Location::checkMethod(std::string & reqMethod)
{
	for (std::vector<std::string>::iterator it = this->mMethod.begin();
			it != this->mMethod.end(); it++)
	{
		if (*it == reqMethod)
			return true;
	}
	return false;
}

bool Location::checkAutoindex(void)
{
	return this->mAutoindex;
}

void Location::setPath(std::string & path)
{
	this->mPath = path;
}

void Location::setIndex(std::string & indexfile)
{
	this->mIndex = indexfile;
}

void Location::setRoot(std::string & rootpath)
{
	this->mRoot = rootpath;
}

void Location::addMethod(std::string & method)
{
	for (std::vector<std::string>::iterator it = this->mMethod.begin();
			it != this->mMethod.end(); it++)
	{
		if (*it == method)
			return ;
	}
	this->mMethod.push_back(method);
}

void Location::setRedirect(std::string & redirection)
{
	this->mRedirect = redirection;
}

void Location::addCgi(std::string & extension, std::string & cgi)
{
	if (this->mCgi.end() == this->mCgi.find(extension))
		this->mCgi[extension] = cgi;
}

void Location::setAutoindex(bool option)
{
	this->mAutoindex = option;
}

void Location::setUpload(std::string & dir)
{
	this->mUpload = dir;
}

void Location::clearMethod(void)
{
	this->mMethod.clear();
}
