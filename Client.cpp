#include "Client.hpp"

Client::Client(int port, std::string password){
    // init some private attribute here
}

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