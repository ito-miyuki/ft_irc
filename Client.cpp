#include "Client.hpp"

Client::Client() : _password(""), _nickname(""), _username(""), _ipA(""), _isRegistered(false), _fd(-1)
{}

Client::Client(int fd, std::string ipAddress) : _password(""), _nickname(""), _username(""), _ipA(ipAddress), _isRegistered(false), _fd(fd)
{}

Client::~Client(){
}

Client::Client(const Client& other) : _password(other.getPassword()), _nickname(other.getNick()), _username(other.getUser()), _ipA(other.getIPa()), _isRegistered(other.isRegistered()), _fd(other.getFd()), _jointChannels(other._jointChannels), _opChannels(other._opChannels)
{}

Client& Client::operator=(const Client& other){
    if (this != &other) {
        _password = other.getPassword();
		_nickname = other.getNick();
		_username = other.getUser();
		_ipA = other.getIPa();
		_isRegistered = other.isRegistered();
		_fd = other.getFd();
		_jointChannels.assign(other._jointChannels.begin(), other._jointChannels.end());
		_opChannels.assign(other._opChannels.begin(), other._opChannels.end());
    }
    return (*this);
}

void	Client::removeChannel(std::string channelName)
{
	std::vector<std::string>::iterator res = std::find(_jointChannels.begin(), _jointChannels.end(), channelName);

	if (res != _jointChannels.end())
		_jointChannels.erase(res);
}

void	Client::removeOpChannel(std::string channelName)
{
	std::vector<std::string>::iterator res = std::find(_opChannels.begin(), _opChannels.end(), channelName);

	if (res != _opChannels.end())
		_opChannels.erase(res);
}
