#ifndef REQUEST_HPP
# define REQUEST_HPP

enum eMethod
{
	UNKNOWN,
	GET,
	POST,
	DELETE,
};

enum eReqReadStatus
{
	READY,
	STARTLINE,
	HEADER,
	BODY,
	COMPLETE,
};

class Request
{
	private:
		eMethod mType;
		std::vector<std::string> mStartLine;
		std::map<std::string, std::string> mHeader;
		std::string mBody;
		std::string mContentType;
		long mContentLength;
		eReqReadStatus mReadStatus;

	public:
		Request(void);
		~Request(void);

		std::string findHeader(std::string const & key);
		void set(std::string const & line);
};

#endif
