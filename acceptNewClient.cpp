#include "Server.hpp"

void	Server::acceptNewClient()
{
	struct sockaddr_in	address;
	struct pollfd		clientFd;
	socklen_t			adLen = sizeof(address);
	
	int clientSocket = accept(getServerFd(), (struct sockaddr *)&(address), &adLen);
	if (clientSocket < 0) {
		std::cerr << "Accept failed" << std::endl;
		return ;
	}

	// to handle multiple clients at the same time || may not be necessary for Linux
	if (fcntl(clientSocket, F_SETFL, O_NONBLOCK) < 0) {
		std::cerr << "fcntl failed" << std::endl;
		return ;
	}

	clientFd.fd = clientSocket; // Add the new client socket
	clientFd.events = POLLIN; // Monitor for incoming data from this client
	clientFd.revents = 0;

	if (_clients.size() == MAX_CLIENTS) {
		std::cerr << "Client amount exceeded" << std::endl;
		return ;
	}

	std::cout << "New client connected: " << clientSocket << std::endl;
	
	Client	newClient(clientSocket, inet_ntoa((address.sin_addr)));

	addClient(newClient);
	_fds.push_back(clientFd);
}


