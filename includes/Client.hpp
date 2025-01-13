#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

class Client
{
	public:
		int fd;
		std::string nickname;
		std::string username;
		bool isAuthenticated;

		Client();
		Client(int fd);
};

#endif
