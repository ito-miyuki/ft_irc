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

int Server::getPort() const {
    return (_port);
}

Client	&Server::getClient(int fd)
{
	std::vector<Client>::iterator end = _clients.end();
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); std::advance(it, 1))
	{
		if (it->getFd() == fd)
			return (*it);
	}
	std::cout << "Couldn't find client" << std::endl;
	return (*end);
}