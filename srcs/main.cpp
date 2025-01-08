#include "SocketServer.hpp"
#include <iostream>
#include <cstdlib>


/*
 * Initialise le serveur avec un port donne 
 * Puis lance le serveur
 * (bloc try)
*/
int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
		return 1;
	}

	int 		port = std::atoi(argv[1]);
	std::string	password = argv[2];

	try
	{
		SocketServer server(port);
		server.start();
	}
	catch (const std::exception& e)
	{
		std::cerr << "[ERROR] " << e.what() << std::endl;
		return 1;
	}

	return 0;
}
