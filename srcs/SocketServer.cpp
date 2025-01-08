#include "SocketServer.hpp"
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>

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
	
	if (bind(server_fd, (struct sockaddr*)&address, sizeof (address)) < 0)
		throw std::runtime_error("Failed to bind socket");

	if (listen(server_fd, 10) < 0)
		throw std::runtime_error("Failed to listen on socket");

	std::cout << "[INFO] Server is listening on port " << port << std::endl;
}

/*
 * Boucle en attendnt une connexion entrante
 * Puis si connexion, affiche l adresse ip + le port
 * Pour l'intsant, ferme la connexion (A MODIFIER, WIP)
*/
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

/*
 * Configure et initialise le socket
 * Puis boucle pour accepter les connexions clients + gerer leurs messages
*/
void SocketServer::start()
{
	setupSocket();
	acceptConnections();
}

