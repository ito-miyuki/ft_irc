#include "Server.hpp"

void	Server::pingMyPong(int cfd, std::string arg)  {
	std::string sent = arg.substr(5);
    std::string msg = "PONG " + sent + "\r\n";
    send(cfd, msg.c_str(), msg.size(), 0);
}
