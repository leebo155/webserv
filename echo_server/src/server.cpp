#include <webserv.hpp>

bool isSvr(std::vector<int> &sockets, int socket)
{
	if (find(sockets.begin(), sockets.end(), socket) != sockets.end())
		return true;
	return false;
}

int main(int argc, char *argv[])
{
	int ret = 0;
	try {
		// parsing port
		std::vector<int> ports;
		if (argc - 1 == 0)
			ports.push_back(8000);
		
		for (int i = 1; i < argc; i++)
		{
			int num = strtod(argv[i], 0);
			if (num <= 0) 
				throw std::runtime_error("port number never be negative or 0");
			ports.push_back(num);
		}

		// creating kqueue fd
		int kqfd = kqueue();
		if (kqfd == -1)
			throw std::runtime_error("kqueue() return -1");
		std::vector<struct kevent> change_list;
		struct kevent event_list[MAX_EVENT];
		std::vector<int> svr_sockets;
		std::map<int, std::string> clt_info;

		// registing socket
		for (size_t i = 0; i < ports.size(); i++)
		{
			int svr_socket = socket(PF_INET, SOCK_STREAM, 0);
			if (svr_socket == -1)
				throw std::runtime_error("socket() return -1");
			
			int svr_port = ports[i];

			struct sockaddr_in addr;
			addr.sin_family = PF_INET;
			addr.sin_port = htons(svr_port);
			addr.sin_addr.s_addr = htonl(INADDR_ANY);
			if (bind(svr_socket, reinterpret_cast<struct sockaddr *>(&addr), sizeof(addr)) == -1)
				throw std::runtime_error("bind(" + std::to_string(svr_port) + ") return -1");

			if (listen(svr_socket, MAX_LISTEN) == -1)
				throw std::runtime_error("listen() return -1");

			if (fcntl(svr_socket, F_SETFL, O_NONBLOCK) == -1)
				throw std::runtime_error("fcntl() return -1");

			struct kevent tmp;
			// EV_SET(&kevent, ident, filter, flags, fflags, data, udata);
			EV_SET(&tmp, svr_socket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
			change_list.push_back(tmp);
			svr_sockets.push_back(svr_socket);
		}

		int new_events;
		struct kevent *cur_event;
		char buffer[BUFFER_SIZE];
		while (true)
		{
			// event check
			new_events = kevent(kqfd, &change_list[0], change_list.size(), event_list, MAX_EVENT, NULL);
			if (new_events == -1)
				throw std::runtime_error("kevent() return -1");
			
			// change_list clear, if change_list is not nullptr, kevent keep checking change_list
			change_list.clear();
			for (int i = 0; i < new_events; i++)
			{
				cur_event = &event_list[i];
				// something wrong...
				if (cur_event->flags & EV_ERROR)
				{
					// if server
					if (isSvr(svr_sockets, cur_event->ident))
						throw std::runtime_error("server socket error");
					else
					{
					// if client
						std::cerr << "client socket error, client disconnected" << std::endl; 
						clt_info.erase(cur_event->ident);
						close(cur_event->ident);
						continue ;
					}
				}

				// can read socket
				if (cur_event->flags & EVFILT_READ)
				{
					// if server
					if (isSvr(svr_sockets, cur_event->ident))
					{
						// accept() connect, registing client socket
						int clt_socket = accept(cur_event->ident, NULL, NULL);
						if (clt_socket == -1)
							throw std::runtime_error("accept() return -1");
						if (fcntl(clt_socket, F_SETFL, O_NONBLOCK) == -1)
						{
							std::cerr << "client socket can't be non-block, client disconnected" << std::endl; 
							clt_info.erase(cur_event->ident);
							close(cur_event->ident);
							continue ;
						}

						struct kevent tmp;
						EV_SET(&tmp, clt_socket, EVFILT_READ | EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);
						clt_info[clt_socket] = "";
						change_list.push_back(tmp);
						std::cout << "accpet() successfully, client connected: " << clt_socket <<std::endl;
					}
					// if client
					else
					{
						// read content
						int len = read(cur_event->ident, buffer, sizeof(buffer));
						if (len == -1)
							continue ;	
						buffer[len] = '\0';
						clt_info[cur_event->ident] += buffer;
					}
				}

				// can write
				if (cur_event->flags & EVFILT_WRITE)
				{
					// keep saving map<int, string> clt_info
					// first line is EOF
					// step1: read first line
					size_t pos = clt_info[cur_event->ident].find_first_of('\n');
					if (pos == std::string::npos)
						continue ;
					// step2: keep reading body
					std::string start_line = "\n" + clt_info[cur_event->ident].substr(0, pos + 1);
					std::string body_line = clt_info[cur_event->ident].substr(pos + 1);
					// step3: check coming EOF line
					pos = body_line.find(start_line);
					if (pos == std::string::npos)
						continue ;
					// step4: create response
					std::string response = "Response\n" + body_line.substr(0, pos);
					
					// step5: send response
					if (write(cur_event->ident, response.c_str(), response.size()) == -1)
						std::cerr << "client socket can't write, client disconnected" << std::endl;
					std::cout << "response send, connection close(" << cur_event->ident << ")" << std::endl;
					clt_info.erase(cur_event->ident);
					close(cur_event->ident);
				}
			}
		}

	} catch (std::exception &e) {
		std::cerr << "Error: " << e.what() << std::endl;
		ret = -1;
	}

	return ret;
}
