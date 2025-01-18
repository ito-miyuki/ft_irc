#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <poll.h> // for pol()
#include <sys/socket.h> // for socket()
#include <netinet/in.h> // for sockaddr_in
#include <unistd.h> // for close()
#include <cstring> // for memset
#include <fcntl.h>
#include <arpa/inet.h> // for inet_ntoa()
#include <sstream>

#define MAX_CLIENTS 100

#include "Client.hpp"
//#include "Channel.hpp"

class Server {
    private:
        int _port; // do we want to name like this instead? port_ 
        std::string _password;
        int _serverFd; // not in use yet
        std::vector<Client> _clients; // client's info
        //std::vector<Channel> _channels; // channel info
        // bool _signal; // for signal handling
		std::vector<pollfd> _fds;
		size_t	_clientAmount;

		void	acceptNewClient();
		void	processClientInput(size_t *clientIndex);
		bool	isRegistered(size_t clientIndex);
		void	registerClient(size_t clientIndex, std::string arg);
		void	processInputData(std::stringstream &ss, size_t clientIndex);
		void	authenticate(Client &client, std::string arg);

		void	registerPassword(Client& client, std::string arg);

    public:
        Server(int port, std::string password); // should it be?: const std::string& password
        ~Server();
        Server(const Server& other);
        Server& operator=(const Server& other);

        int runServer();
		int createSocket();
        void handleNickCommand(int clientSocket, const std::string& command);

        int getPort() const;
};