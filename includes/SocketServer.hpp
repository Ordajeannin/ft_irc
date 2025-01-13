#ifndef SOCKETSERVER_HPP
#define SOCKETSERVER_HPP

#include <string>
#include <netinet/in.h> // ?
#include <stdexcept>
#include <iostream>
#include "Client.hpp"
#include <map>
#include <vector>

class SocketServer
{
	private:
		int server_fd;
		sockaddr_in address;
		int port;
		std::map<std::string, std::vector<Client*> > channels;

		void setupSocket();
		void acceptConnections();
		void broadcastToChannel(const std::string& channel, const std::string& message, Client* sender);

	public:
		SocketServer(int port);
		~SocketServer();

		void start();
};

#endif 
