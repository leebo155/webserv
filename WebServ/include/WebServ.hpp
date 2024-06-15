#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <iostream>

# include <vector>
# include <map>
# include <fstream>
# include <exception>
# include <sys/socket.h>
# include <netinet/in.h>
# include <fcntl.h>
# include "ft.hpp"
# include "Configure.hpp"
# include "Server.hpp"
//# include "Connection.hpp"
# include "Kqueue.hpp"
# include "Logger.hpp"
# include "Message.hpp"

# define DEFAULT_CONFIG "conf/default.conf"
# define PORT_MIN 1024
# define PORT_MAX 49151
# define LISTEN_MAX 10


class WebServ {
	private:
		std::vector<Server> mServers;
		std::map<int, std::vector<int> > mPortGroup;
		std::map<std::string, std::string> mMIMEType;
//		std::vector<Connection> mConnection;
		Kqueue mKqueue;
		Logger mLogger;

		void printAll(void);

		void validConfig(std::string contents);
		void parseConfig(std::string & contents);
		void createMIMEType(void);
		void listenServer(void);
		std::string findMIMEType(std::string const & file);

	public:
		WebServ(void);
		~WebServ(void);

		void configure(std::string const & config);
		void activate(char *envp[]);
};

#endif
