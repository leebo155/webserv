#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <vector>
# include <fstream>
# include <exception>
# include "ft.hpp"
# include "Configure.hpp"
# include "Server.hpp"
# include "Connection.hpp"
# include "Kqueue.hpp"
# include "Logger.hpp"

# define INT_MIN -2147483648
# define INT_MAX 2147483647
# define PORT_MIN 1024
# define PORT_MAX 49151

class WebServ {
	private:
		std::vector<Server> mServers;
		std::vector<Connection> mConnection;
		Kqueue mKqueu;
		Logger mLogger;

		void validConfig(std::string contents);
		void parseConfig(std::string & contents);

	public:
		WebServ(void);
		WebServ(const std::string & config);
		~WebServ(void);

		void configure(const std::string & config);
		void activate(std::vector<std::string> envp);
};

#endif
