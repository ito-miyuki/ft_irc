#include "Server.hpp"

int Server::createSocket(){

	int socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketFd == -1) {
		return -1;
	}
	return socketFd;
}

int Server::runServer() {

	int _serverFd = createSocket();
	if (_serverFd == -1) {
		std::cerr << "Failed to create socket." << std::endl;
		return 1;
	}

	int	optVal = 1;
	if (setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof(optVal)) < 0) { // EDIT: added this according to irc tutorial to reuse local addresses
		std::cerr << "setsockopt failed." << std::endl;
		close(_serverFd);
		return 1;
	}
	struct sockaddr_in serverAddress; // Struct to hold server address information
    memset(&serverAddress, 0, sizeof(serverAddress)); // Zero out the memory for the struct
    serverAddress.sin_family = AF_INET; // Set to IPv4
    serverAddress.sin_port = htons(_port); // Convert the port number to network byte order
    serverAddress.sin_addr.s_addr = INADDR_ANY; // Allow connections from any network interface

	if (bind(_serverFd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
		std::cerr << "Bind failed." << std::endl;
		close(_serverFd);
		return 1;
	}

	if (listen(_serverFd, MAX_CLIENTS) < 0) { // EDIT: changed 5 to 100 to handle max client no
		std::cerr << "Listen failed." << std::endl;
		close(_serverFd);
		return 1;
	}

	std::cout << "Server listening on port: " << getPort() << std::endl;

	pollfd serverFd;
	serverFd.fd = _serverFd; // Monitor the server socket for new connections
    serverFd.events = POLLIN; // POLLIN: Check for incoming data or connection requests
	serverFd.revents = 0; // EDIT: added revents to fill struct fields
    _fds.push_back(serverFd); // Add the server socket to the list of monitored sockets

	while (true) { // while we don't recieve signal
		int pollCount = poll(_fds.data(), _fds.size(), -1);
		
		if (pollCount < 0) {
			if (errno == EINTR) {
				std::cout << "Poll interrupted by signal" << std::endl;
			} else {
			std::cerr << "Poll failed" << std::endl;
			} 
			break;
		}
		
		for (size_t i = 0; i < _fds.size(); ++i) {

			if (_fds[i].revents & POLLIN)
			{
				if (_fds[i].fd == _serverFd) {
					std::cout << "server socket before: " << _serverFd << std::endl;
					acceptNewClient();
				}
				else {
					processClientInput(&i);
				}
			}
		}
	}

	for (std::vector<pollfd>::iterator it = _fds.begin(); it != _fds.end(); ++it) {
		close(it->fd);
	}
	close(_serverFd);
	return 0;
}
