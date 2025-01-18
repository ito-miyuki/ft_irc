#include "Client.hpp"

Client::Client(int fd, std::string ipAddress) : _ipA(ipAddress), _isRegistered(false), _fd(fd)
{}

Client::~Client(){
    // anytging to delete?
}

Client::Client(const Client& other){
    (void)other;
}

Client& Client::operator=(const Client& other){
    if (this != &other) {
        (void)other;
    }
    return (*this);
}