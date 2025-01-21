#include "Server.hpp"

Server::Server(int port, std::string password): _port(port), _password(password), _clientAmount(0) {}

Server::~Server(){
    // anytging to delete?
}

Server::Server(const Server& other){
    _port = other._port;
    _password = other._password;
}

Server& Server::operator=(const Server& other){
    if (this != &other) {
        _port = other._port;
        _password = other._password;
    }
    return (*this);
}

void    Server::decrementClientAmount() {
    if (_clientAmount == 0)
        return ;
    _clientAmount--;
}

std::vector<Client>::iterator	&Server::getClient(int fd)
{
	std::vector<Client>::iterator end = _clients.end();
	for (std::vector<Client>::iterator it = _clients.begin(); it != end; std::advance(it, 1))
	{
		if (it->getFd() == fd)
			return (it);
	}
	std::cout << "Couldn't find client" << std::endl;
	return (end);
}

std::vector<Channel>::iterator	&Server::getChannel(std::string name)
{
	std::vector<Channel>::iterator end = _channels.end();
	for (std::vector<Channel>::iterator it = _channels.begin(); it != end; std::advance(it, 1))
	{
		if (it->getChannelName() == name)
			return (it);
	}
	return (end);
}

bool Server::_signal = false;

void Server::setSignal(bool value) {
        _signal = value;
}