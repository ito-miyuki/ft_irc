#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <poll.h> // for pol()

// #include "Client.hpp"

class Server {
    private:
        int _port; // do we want to name like this instead? port_ 
        std::string _password;
        // int _socketFd; // not in use yet
        // std::vector<Client> _clients; // client's info
        // std::vector<Channel> _channels; // channel info
        // bool _signal; // for signal handling
		std::vector<pollfd> fds;

    public:
        Server(int port, std::string password); // should it be?: const std::string& password
        ~Server();
        Server(const Server& other);
        Server& operator=(const Server& other);

        int runServer();
        void handleNickCommand(int clientSocket, const std::string& command);

        int getPort() const;
};