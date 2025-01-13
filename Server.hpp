#pragma once
#include <string>

class Server {
    private:
        int _port; // do we want to name like this instead? port_ 
        std::string _password;
        // int _socketFd; // not in use yet

    public:
        Server(int port, std::string password); // should it be?: const std::string& password
        ~Server();
        Server(const Server& other);
        Server& operator=(const Server& other);

        int runServer(Server *server);

        int getPort() const;
};