#include "Server.hpp"

Server::Server(int port, std::string password): _port(port), _password(password) {}

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

int Server::getPort() const {
    return (_port);
}