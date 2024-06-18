#ifndef CONNECTION_HPP
# define CONNECTION_HPP

# include <sys/time.h>
# include "Request.hpp"
# include "Response.hpp"

# define CRLF "\r\n"
# define BUFFER_SIZE 4096
# define OVERTIME 15 

enum eValidStatus
{
	READY,
	METHOD,

};

class Connection
{
	private:
		int mSocket;
		int mServerPort;
		Server *mServer;
		Request mRequest;
		Response mResponse;
		std::string  mAbsolutePath;

		eValidStatus mValidStatus;

		std::string mRemainStr;
		struct timeval mTime;

		void renewTime(void);

	public:
		Connection(void);
		Connection(int socket, int svr_port);
		~Connection(void);

		Server *getServer(void);
		std::string getHost(void);

		void setServer(Server *svr);

		void readRequest(void);
		void validRequest(void);
		void writeResponse(void);
		void close(void);
		void access(void);
		bool checkComplete(void);
		bool checkOvertime(void);

};

#endif
