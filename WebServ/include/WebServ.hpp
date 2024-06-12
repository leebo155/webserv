#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <iostream>

# include <vector>
# include <fstream>
# include <exception>
# include "ft.hpp"
# include "Configure.hpp"
# include "Server.hpp"
//# include "Connection.hpp"
//# include "Kqueue.hpp"
//# include "Logger.hpp"

# define DEFAULT_CONFIG "conf/default.conf"
# define PORT_MIN 1024
# define PORT_MAX 49151

class WebServ {
	private:
		std::vector<Server> mServers;
//		std::vector<Connection> mConnection;
//		Kqueue mKqueu;
//		Logger mLogger;

		void printAll(void);

		void validConfig(std::string contents);
		void parseConfig(std::string & contents);

	public:
		WebServ(void);
		WebServ(const std::string & config);
		~WebServ(void);

		void configure(const std::string & config);
		void activate(char *envp[]);
};

#endif
