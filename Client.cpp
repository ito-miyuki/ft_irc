#include "Client.hpp"

/* Client::Client() : _password(""), _nickname(""), _username(""), _ipA(""), _isRegistered(false), _fd(-1)
{} */

Client::Client(int fd, std::string ipAddress) : _password(""), _nickname(""), _username(""), _ipA(ipAddress), _isRegistered(false), _fd(fd)
{}

Client::~Client(){
    // anytging to delete?
}

Client::Client(const Client& other) : _password(other.getPassword()), _nickname(other.getNick()), _username(other.getUser()), _ipA(other.getIPa()), _isRegistered(other.isRegistered()), _fd(other.getFd())
{}

Client& Client::operator=(const Client& other){
    if (this != &other) {
        _password = other.getPassword();
		_nickname = other.getNick();
		_username = other.getUser();
		_ipA = other.getIPa();
		_isRegistered = other.isRegistered();
		_fd = other.getFd();
    }
    return (*this);
}

