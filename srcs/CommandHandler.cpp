
/*
#include "CommandHandler.hpp"
#include <iostream>
#include <sys/socket.h>


void CommandHandler::handleCommand(Client &client, const std::string &msg)
{
	std::string command = msg.substr(0, msg.find(' '));
	std::string params = msg.substr(msg.find(' ') + 1);

	if (command == "PASS")
		std::cout << "[INFO] PASS command received" << std::endl;
	else if (command == "NICK")
	{
		client.nickname = params;
		std::cout << "[INFO] Client set nickname to: " << client.nickname << std::endl;
	}
	else if (command == "USER")
	{
		client.username = params;
		client.isAuthenticated = true;
		std::cout <<"[INFO] Client authenticated: " << client.nickname << std::endl;
		
		std::string welcome = ":server 001 " + client.nickname + " :Welcome to the IRC server " + client.nickname + "\r\n";
		send(client.fd, welcome.c_str(), welcome.length(), 0);
	}
	else
		std::cout << "[INFO] Unknown command: " << command << std::endl;
}


#include "CommandHandler.hpp"
#include <iostream>
#include <sstream> // Pour découper les paramètres

void CommandHandler::handleCommand(Client& client, const std::string& message) {
	std::istringstream iss(message);
//    std::string command;
    iss >> command; // Extraire la commande

	std::string command = message.substr(0, message.find(' '));
	std::string params = message.substr(message.find(' ') + 1);
    if (command == "PASS") {
        // Ignorer PASS pour l'instant (mot de passe déjà accepté)
        std::cout << "[INFO] PASS command received" << std::endl;
    }
    else if (command == "NICK") {
        client.nickname = params;
		if (!client.nickname.empty())
        	std::cout << "[INFO] Client set nickname to: " << client.nickname << std::endl;
    }
    else if (command == "USER") {
        std::string username, host, servername, realname;
        iss >> username >> host >> servername; // Extraire les paramètres
        std::getline(iss, realname);          // Lire le vrai nom (qui peut contenir des espaces)
        realname = realname.substr(1);        // Enlever le premier caractère ':' de realname

        client.username = username;
        client.isAuthenticated = true; // Authentification minimale réussie
   //     std::cout << "[INFO] Client authenticated: " << client.nickname << std::endl;

        // Message de bienvenue
		if (!client.nickname.empty())
		{
        	std::string welcome = ":server 001 " + client.nickname +
				" :Welcome to the IRC server " + client.nickname + "\r\n";
        	send(client.fd, welcome.c_str(), welcome.length(), 0);
		    std::cout << "[INFO] Client authenticated: " << client.nickname << std::endl;
		}
    }
    else if (command == "USERHOST") {
        // Répondre basiquement à USERHOST pour éviter les erreurs
        std::string response = ":server 302 " + client.nickname + " :No userhost information available\r\n";
        send(client.fd, response.c_str(), response.length(), 0);
        std::cout << "[INFO] USERHOST command received, sent dummy response" << std::endl;
    }
	else if (command == "PING")
	{
		std::string response = "PONG " + params + "\r\n";
		send(client.fd, response.c_str(), response.length(), 0);
		std::cout << "[INFO] PING command received, sent PONG" << std::endl;
	}
	else if (command == "CAP")
	{
		std::string response = "CAP * LS :\r\n";
		send(client.fd, response.c_str(), response.length(), 0);
		std::cout << "[INFO] CAP command received, sent dummy response" << std::endl;
	}
	else if (command == "JOIN")
	{
    	std::string channel = params; // Nom du canal

	    // Vérifier que le canal commence par '#' ou '&' (selon la norme IRC)
	    if (channel.empty() || (channel[0] != '#' && channel[0] != '&')) 
		{
    	    std::string errorMsg = ":server 403 " + client.nickname + " " + channel + " :Invalid channel name\r\n";
        	send(client.fd, errorMsg.c_str(), errorMsg.length(), 0);
        	std::cout << "[INFO] Client " << client.nickname << " tried to join an invalid channel: " << channel << std::endl;
        	return;
    	}

    // Ajouter le canal s'il n'existe pas encore
	    if (channels.find(channel) == channels.end())
  	      channels[channel] = std::vector<Client*>();

    // Vérifier si le client est déjà dans le canal
	    std::vector<Client*>& members = channels[channel];
	    if (std::find(members.begin(), members.end(), &client) == members.end())
    	    members.push_back(&client); // Ajouter le client au canal

    // Confirmer au client qu'il a rejoint le canal
	    std::string joinMsg = ":" + client.nickname + " JOIN " + channel + "\r\n";
	    send(client.fd, joinMsg.c_str(), joinMsg.length(), 0);

    // Informer les autres membres du canal
   		for (Client* member : members) {
        	if (member != &client) {
            	send(member->fd, joinMsg.c_str(), joinMsg.length(), 0);
        	}
    	}

	    std::cout << "[INFO] Client " << client.nickname << " joined channel: " << channel << std::endl;
	}
	else
        std::cout << "[INFO] Unknown command: " << command << std::endl;
}
*/

#include "CommandHandler.hpp"
#include "Client.hpp"
#include <sstream> // Pour extraire les parties du message
#include <algorithm> // Pour std::find
#include <iostream> // Pour les logs
#include <map>
#include <vector>
#include <sys/socket.h>

void CommandHandler::handleCommand(Client& client, const std::string& message, std::map<std::string, std::vector<Client*> >& channels)
{
    // Extraire la commande et les paramètres du message
    std::istringstream iss(message);
    std::string command;
    iss >> command; // Extraire la commande
    std::string params;
    std::getline(iss, params);
    if (!params.empty() && params[0] == ' ') {
        params = params.substr(1); // Retirer l'espace initial
    }

    if (command == "PASS") {
        std::cout << "[INFO] PASS command received" << std::endl;
        // Ignorer pour l'instant
    } 
    else if (command == "NICK") {
        if (params.empty()) {
            std::cout << "[ERROR] NICK command missing parameter" << std::endl;
            return;
        }
        client.nickname = params;
        std::cout << "[INFO] Client set nickname to: " << client.nickname << std::endl;
    } 
    else if (command == "USER") {
        std::istringstream paramStream(params);
        std::string username, host, servername, realname;
        paramStream >> username >> host >> servername;
        std::getline(paramStream, realname);
        if (!realname.empty() && realname[0] == ':') {
            realname = realname.substr(1); // Retirer le ':' initial
        }

        client.username = username;
        client.isAuthenticated = true;
        std::cout << "[INFO] Client authenticated: " << client.nickname << std::endl;

        // Envoyer le message de bienvenue si le pseudo est défini
        if (!client.nickname.empty()) {
            std::string welcome = ":server 001 " + client.nickname +
                                  " :Welcome to the IRC server " + client.nickname + "\r\n";
            send(client.fd, welcome.c_str(), welcome.length(), 0);
        }
    } 
    else if (command == "JOIN") {
        if (params.empty()) {
            std::string errorMsg = ":server 461 " + client.nickname + " JOIN :Not enough parameters\r\n";
            send(client.fd, errorMsg.c_str(), errorMsg.length(), 0);
            std::cout << "[ERROR] JOIN command missing parameter" << std::endl;
            return;
        }

        std::string channel = params;
        if (channel[0] != '#' && channel[0] != '&') {
            std::string errorMsg = ":server 403 " + client.nickname + " " + channel + " :Invalid channel name\r\n";
            send(client.fd, errorMsg.c_str(), errorMsg.length(), 0);
            std::cout << "[ERROR] Client " << client.nickname << " tried to join invalid channel: " << channel << std::endl;
            return;
        }

        // Ajouter le client au canal
        if (channels.find(channel) == channels.end()) {
            channels[channel] = std::vector<Client*>();
        }
        std::vector<Client*>& members = channels[channel];
        if (std::find(members.begin(), members.end(), &client) == members.end()) {
            members.push_back(&client);
        }

        // Envoyer un message de confirmation au client
        std::string joinMsg = ":" + client.nickname + " JOIN " + channel + "\r\n";
        send(client.fd, joinMsg.c_str(), joinMsg.length(), 0);

        // Informer les autres membres du canal
        for (std::vector<Client*>::iterator it = members.begin(); it != members.end(); ++it) {
            if (*it != &client) {
                send((*it)->fd, joinMsg.c_str(), joinMsg.length(), 0);
            }
        }

        std::cout << "[INFO] Client " << client.nickname << " joined channel: " << channel << std::endl;
    } 
    else if (command == "PING") {
        std::string response = "PONG " + params + "\r\n";
        send(client.fd, response.c_str(), response.length(), 0);
        std::cout << "[INFO] PING command received, sent PONG" << std::endl;
    } 
    else {
        std::cout << "[INFO] Unknown command: " << command << std::endl;
    }
}

