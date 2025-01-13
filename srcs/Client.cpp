#include "Client.hpp"

Client::Client() : fd(-1), isAuthenticated(false) {}
Client::Client(int fd) : fd(fd), isAuthenticated(false) {}

