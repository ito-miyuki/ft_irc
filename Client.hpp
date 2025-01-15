#pragma once
#include <string>
#include <vector>
//#include "Channel.hpp"

class Client {
    private:
        std::string _password;
        std::string _nickname;
        std::string _username;
        bool isRegistered;
        // int _fd; // fd for indentifying the matching poll struct
        // std::vector<Channel> jointChannels; // channels user is part of
        // std::vector<Channel> opChannels; // channels where user is the moderator
    public:
        Client(int port, std::string password); // should it be?: const std::string& password
        ~Client();
        Client(const Client& other);
        Client& operator=(const Client& other);
};