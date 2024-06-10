#ifndef SERVER_HPP
# define SERVER_HPP

# include "ft.hpp"
# include "Location.hpp"

# define DEFAULT_SERVERNAME "WebServ"

class Server
{
	private:
		int mPort;
		int mSocket;
		int mBodySize;
		std::string mHost;
		std::string mServerName;
		Response mErrorPage;
		std::vector<Location> mLocation;

	public:
		Server(void);
		~Server(void);

		int	getPort(void);
		int	getSocket(void);
		std::string & getServerName(void);
		Location *getLocation(size_t idx);
		size_t	getLocationSize(void)

		void setPort(int port);
		void setSocket(int socket);
		void setBodySize(std::string & dataSize);
		void setHost(std::string & host);
		void setServerName(std::string & servername);
		void setErrorPage(std::string & file);
		void addLocation(Location & lct);
};

#endif
