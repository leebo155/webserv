#include "WebServ.hpp"
#include <iostream>

int main(int argc, char *argv[], char *envp[])
{
	try {
		WebServ manager;
		if (argc - 1 == 0)
			manager.configure(DEFAULT_CONFIG);
		else
			manager.configure(argv[1]);
		manager.activate(envp);
	} catch (std::exception &e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}
	return 0;
}
