#pragma once
#include <string>

class Client {
    private:
        std::string _password;
        std::string _nickname;
        std::string _username;
        bool isRegistered;
    public:
        Client(int port, std::string password); // should it be?: const std::string& password
        ~Client();
        Client(const Client& other);
        Client& operator=(const Client& other);
};