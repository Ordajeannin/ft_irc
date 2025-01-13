#ifndef COMMANDHANDLER_HPP
#define COMMANDHANDLER_HPP

#include <string>
#include "Client.hpp"
#include <map>
#include <vector>

class CommandHandler
{
	public:
		static void handleCommand(Client& client, const std::string& message, std::map<std::string, std::vector<Client*> >& channels);
};

#endif
