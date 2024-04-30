#include <webserv.hpp>

int main(int argc, char *argv[])
{
	try {
		if (argc - 1 != 1)
			throw std::runtime_error("require port number");

		// socket()
		int socket_fd = socket(PF_INET, SOCK_STREAM, 0);
		if (socket_fd == -1)
			throw std::runtime_error("socket() return -1");
			
		// connect()
		int num = strtod(argv[1], NULL);
		struct sockaddr_in addr;
		addr.sin_family = PF_INET;
		addr.sin_port = htons(num);
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
		if (connect(socket_fd, reinterpret_cast<struct sockaddr *>(&addr), sizeof(addr)) == -1)
			throw std::runtime_error("connect() return -1");
		std::cout << "connect() successfully" << std::endl;

		// eof line read and send
		std::cout << "Request" << std::endl;
		char	buffer[BUFFER_SIZE];
		int len = read(0, buffer, sizeof(buffer));
		if (len == -1)
			throw std::runtime_error("read() failed!");
		buffer[len] = '\0';
		std::string eofline = buffer;
		if (write(socket_fd, buffer, len) == -1)
			throw std::runtime_error("write() failed!");

		// read and write
		while (true)
		{
			len = read(0, buffer, sizeof(buffer));
			if (len == -1)
				throw std::runtime_error("read() failed!");
			buffer[len] = '\0';
			if (write(socket_fd, buffer, len) == -1)
				throw std::runtime_error("write() failed!");
			if (buffer == eofline)
				break ;
		}

		// server sending message read and print
		len = read(socket_fd, buffer, sizeof(buffer));
		if (len == -1)
			throw std::runtime_error("read() failed!");
		buffer[len] = '\0';
		std::cout << buffer << std::endl;
		close(socket_fd);
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;	
	}
	return 0;
}
