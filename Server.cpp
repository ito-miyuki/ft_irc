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

int	Server::getClientIndex(int fd)
{
	int	index = 0;
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); std::advance(it, 1))
	{
		if (it->getFd() == fd)
		{
			return (index);
		}
		index++;
	}
	return (-1);
}

int	Server::getChannelIndex(std::string name)
{
	int	index = 0;
	for (std::vector<Channel>::iterator it = _channels.begin(); it != _channels.end(); std::advance(it, 1))
	{
		if (it->getChannelName() == name)
		{
			return (index);
		}
		index++;
	}
	return (-1);
}

bool Server::_signal = false;

void Server::setSignal(bool value) {
        _signal = value;
}

// this is a temporary solution
Channel* Server::getChannelObj(const std::string& channelName) {
	// is auto ok?
    auto it = std::find_if(_channels.begin(), _channels.end(),
        [&channelName](const Channel& ch) { return ch.getChannelName() == channelName; });

    if (it != _channels.end()) {
        return &(*it);
    }
    return nullptr;
}

Client* Server::getClientObjByFd(int fd) {
	for (std::vector<Client>::iterator ite = _clients.begin(); ite != _clients.end(); ++ite) {
		if (ite->getFd() == fd) {
			return &(*ite);
		}
	}
	return nullptr;
}