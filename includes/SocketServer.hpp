#ifndef SOCKETSERVER_HPP
#define SOCKETSERVER_HPP

#include <string>
#include <netinet/in.h> // ?
#include <stdexcept>
#include <iostream>

class SocketServer
{
	private:
		int server_fd;
		sockaddr_in address;
		int port;

		void setupSocket();
		void acceptConnections();

	public:
		SocketServer(int port);
		~SocketServer();

		void start();
};

#endif 
