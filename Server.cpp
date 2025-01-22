#include "Server.hpp"

Server::Server(int port, std::string password): _port(port), _password(password), _clientAmount(0) {}

Server::Server(const Server& other) : _port(other.getPort()), _password(other.getPassword()), _serverFd(other.getServerFd()), _clients(other._clients), _channels(other._channels), _fds(other._fds), _clientAmount(other.getClientAmount())
{}

Server& Server::operator=(const Server& other){
    if (this != &other) {
        _port = other.getPort();
        _password = other.getPassword();
		_serverFd = other.getServerFd();
		_clients.assign(other._clients.begin(), other._clients.end());
		_channels.assign(other._channels.begin(), other._channels.end());
		_fds.assign(other._fds.begin(), other._fds.end());
		_clientAmount = other.getClientAmount();
    }
    return (*this);
}

Server::~Server() {}

void    Server::decrementClientAmount() {
    if (_clientAmount == 0)
        return ;
    _clientAmount--;
}

Client	&Server::getClient(int fd)
{
	std::vector<Client>::iterator end = _clients.end();
	for (std::vector<Client>::iterator it = _clients.begin(); it != end; std::advance(it, 1))
	{
		if (it->getFd() == fd)
			return (*it);
	}
	std::cout << "Couldn't find client" << std::endl;
	return (*end); // this can cause problems....
}

Channel	&Server::getChannel(std::string name)
{
	std::vector<Channel>::iterator end = _channels.end();
	for (std::vector<Channel>::iterator it = _channels.begin(); it != end; std::advance(it, 1))
	{
		if (it->getChannelName() == name)
			return (*it);
	}
	return (*end); // this can cause problems....
}

bool Server::_signal = false;

void Server::setSignal(bool value) {
        _signal = value;
}