#include "SocketServer.hpp"
#include "Client.hpp"
#include "CommandHandler.hpp"
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <poll.h>
#include <vector>
#include <map>
#include <cstdio>

/*
 * Initialise le socket a un etat invalide (-1)
 * Initialise sockaddr_in (necessaire pour bind, listen, accept...)
 * Puis
 *   - definit le type d'adresse (IPV4)
 *   - accepte les connexions sur toutes les interfaces reseau
 *   - s'assure du bon ordre de lecture des octets (tah b2br)
*/
SocketServer::SocketServer(int port) : server_fd(-1), port(port)
{
	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);
}

/*
 * Ferme proprement le socket (si le serveur est actif)
*/
SocketServer::~SocketServer()
{
	if (server_fd != -1)
	{
		close(server_fd);
		std::cout << "[INFO] server socket closed" << std::endl;
	}
}

/*
 * Cree un socket TCP
 * puis "l'associe" a l'adresse et au port
 * puis le configure pour "ecouter" les connexions entrantes
*/
void SocketServer::setupSocket()
{
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	
	if (server_fd == -1)
		throw std::runtime_error("Failed to create socket");

	int opt = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		throw std::runtime_error("Failed to set socket options.");
	
	if (bind(server_fd, (struct sockaddr*)&address, sizeof (address)) < 0)
		throw std::runtime_error("Failed to bind socket");

	if (listen(server_fd, 10) < 0)
		throw std::runtime_error("Failed to listen on socket");

	std::cout << "[INFO] Server is listening on port " << port << std::endl;
}


/*
void SocketServer::acceptConnections()
{
	std::vector<pollfd> fds;
	std::map<int, Client> clients;

	pollfd server_pollfd = {server_fd, POLLIN, 0};
	fds.push_back(server_pollfd);

	while (1)
	{
		int ready = poll(fds.data(), fds.size(), -1);
		if (ready < 0)
		{
			perror("poll");
			break ;
		}

		for (size_t i = 0; i < fds.size(); ++i)
		{
			if (fds[i].fd == server_fd && (fds[i].revents & POLLIN))
			{
				sockaddr_in 	client_address;
				socklen_t 		client_len = sizeof(client_address);
				int 			client_fd = accept(server_fd, (struct sockaddr*)&client_address, &client_len);

				if (client_fd < 0)
				{
					std::cerr << "[ERROR] Failed to accept connection." << std::endl;
					continue ;
				}
				std::cout << "[INFO] new client connected: " << client_fd << std::endl;

				clients[client_fd] = Client(client_fd);
				pollfd client_pollfd = {client_fd, POLLIN, 0};
				fds.push_back(client_pollfd);
			}
			else if (fds[i].revents & POLLIN)
			{
				char buffer[1024];
				ssize_t bytes_read = recv(fds[i].fd, buffer, sizeof(buffer) - 1, 0);

				if (bytes_read <= 0)
				{
					std::cout << "[INFO] Client disconnected: " << fds[i].fd << std::endl;
					close(fds[i].fd);
					clients.erase(fds[i].fd);
					fds.erase(fds.begin() + i);
					--i;
				}
				else
				{
					buffer[bytes_read] = '\0';
					std::string message(buffer);
					std::cout << "[INFO] Received from client " << fds[i].fd << ": " << message << std::endl;

					CommandHandler::handleCommand(clients[fds[i].fd], message);
//					std::string response = "Server echo: " + std::string(buffer);
//					send(fds[i].fd, response.c_str(), response.length(), 0);
				}
			}
		}
	}
	for (size_t i = 0; i < fds.size(); ++i)
	{
		close(fds[i].fd);
	}
}
*/

void SocketServer::acceptConnections()
{
	std::vector<pollfd> fds;
	std::map<int, Client> clients;
	std::map<std::string, std::vector<Client*> > channels; // Ajouter la gestion des canaux

	pollfd server_pollfd = {server_fd, POLLIN, 0};
	fds.push_back(server_pollfd);

	while (1)
	{
		int ready = poll(fds.data(), fds.size(), -1);
		if (ready < 0)
		{
			perror("poll");
			break ;
		}

		for (size_t i = 0; i < fds.size(); ++i)
		{
			if (fds[i].fd == server_fd && (fds[i].revents & POLLIN))
			{
				sockaddr_in 	client_address;
				socklen_t 		client_len = sizeof(client_address);
				int 			client_fd = accept(server_fd, (struct sockaddr*)&client_address, &client_len);

				if (client_fd < 0)
				{
					std::cerr << "[ERROR] Failed to accept connection." << std::endl;
					continue ;
				}
				std::cout << "[INFO] New client connected: " << client_fd << std::endl;

				clients[client_fd] = Client(client_fd);
				pollfd client_pollfd = {client_fd, POLLIN, 0};
				fds.push_back(client_pollfd);
			}
			else if (fds[i].revents & POLLIN)
			{
				char buffer[1024];
				ssize_t bytes_read = recv(fds[i].fd, buffer, sizeof(buffer) - 1, 0);

				if (bytes_read <= 0)
				{
					std::cout << "[INFO] Client disconnected: " << fds[i].fd << std::endl;
					close(fds[i].fd);
					clients.erase(fds[i].fd);
					fds.erase(fds.begin() + i);
					--i;
				}
				else
				{
					buffer[bytes_read] = '\0';
					std::string message(buffer);
					std::cout << "[INFO] Received from client " << fds[i].fd << ": " << message << std::endl;

					// Passer la map des channels au gestionnaire de commandes
					CommandHandler::handleCommand(clients[fds[i].fd], message, channels);
				}
			}
		}
	}
	for (size_t i = 0; i < fds.size(); ++i)
	{
		close(fds[i].fd);
	}
}


void SocketServer::broadcastToChannel(const std::string& channel, const std::string& message, Client* sender)
{
    if (channels.find(channel) == channels.end())
	{    
		return ; // Le canal n'existe pas
	}

	std::vector<Client*>& members = channels[channel];

	for (std::vector<Client*>::iterator it = members.begin(); it != members.end(); ++it) 
	{
    	Client* member = *it;
   		if (member != sender)
        	send(member->fd, message.c_str(), message.length(), 0);
    }
}

/*
 * Boucle en attendnt une connexion entrante
 * Puis si connexion, affiche l adresse ip + le port
 * Pour l'intsant, ferme la connexion (A MODIFIER, WIP)
*/
/*
void SocketServer::acceptConnections() 
{
	int 			client_fd;
	sockaddr_in		client_address;
	socklen_t		client_len = sizeof(client_address);
														
	while (1)
	{
		client_fd = accept(server_fd, (struct sockaddr*)&client_address, &client_len);
		if (client_fd < 0)
		{
			std::cerr << "[ERROR] Failed to accept connection." << std::endl;
			continue;
		}
		std::cout << "[INFO] New connection accepted from " 
			<< inet_ntoa(client_address.sin_addr)
			<< ":" << ntohs(client_address.sin_port) << std::endl;
		close(client_fd);
	}
}
*/

/*
 * Configure et initialise le socket
 * Puis boucle pour accepter les connexions clients + gerer leurs messages
*/
void SocketServer::start()
{
	setupSocket();
	acceptConnections();
}

