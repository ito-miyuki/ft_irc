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


Channel* Server::getChannelObj(const std::string& channelName) {
    for (std::vector<Channel>::iterator ite = _channels.begin(); ite != _channels.end(); ++ite) {
        if (ite->getChannelName() == channelName) {
            return &(*ite);
        }
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

int Server::getUserFdbyNick(const std::string& nickName) {
    for (std::vector<Client>::iterator ite = _clients.begin(); ite != _clients.end(); ++ite) {
        if (ite->getNick() == nickName) {
            return ite->getFd();
        }
    }
    return -1;
}

bool Server::channelExist(const std::string& channelName) {
    std::cout << "You are in channelExist()" << std::endl; // delete it 
    std::cout << "Total channels in _channels: " << _channels.size() << std::endl; // delete it

    for (std::vector<Channel>::iterator ite = _channels.begin(); ite != _channels.end(); ++ite) {
        std::cout << "channel name that I am looking at is: " << ite->getChannelName() << std::endl;
        if (ite->getChannelName() == channelName) {
            std::cout << "Channel name exist" << std::endl; // do something
            return true;
        }
    }
    return false;
}

int Server::getUserFdByNick(const std::string& nickName) {
    std::cout << "you are in getUserFdByNick()" << std::endl; // delete it

    for (std::vector<Client>::iterator ite = _clients.begin(); ite != _clients.end(); ++ite) {
        if (ite->getNick() == nickName) {
            return ite->getFd();
        }
    }

    return -1;
}

bool Server::isUserInChannel(const std::string& channelName, int userFd) {

    for (std::vector<Channel>::iterator ite = _channels.begin(); ite != _channels.end(); ++ite) {
        if (ite->getChannelName() == channelName) {
            std::vector<int> jointClients = ite->getJointClients();
            std::vector<int> operators = ite->getOps();

            std::vector<int>::iterator found = std::find(jointClients.begin(), jointClients.end(), userFd);
            std::vector<int>::iterator foundInOps = std::find(operators.begin(), operators.end(), userFd);
            
            if(found != jointClients.end() || foundInOps != operators.end()) {
                return true;
            }
            return false;
        }
    }
    std::cout << "The channel doesn't exist" << std::endl; // change the message
    return false;
}