#include "../Server.hpp"

static void handleSignal(int signal){
	switch (signal) {
		case SIGINT:
			std::cout << "SIGINT recieved.\nShutting down gracefully..." << std::endl;
			break;
		case SIGQUIT:
			std::cout << "SIGQUIT recieved.\nTerminating process..." << std::endl;
			break;
		default:
			break;
	}
	Server::setSignal(true);
}

void Server::setServerFd(){
	_serverFd = socket(AF_INET, SOCK_STREAM, 0);
}

int Server::runServer() {

	// handle signal here.
	signal(SIGINT, handleSignal); // ctrl + c
	signal(SIGQUIT, handleSignal); // 'ctrl + \'
	signal(SIGTERM, handleSignal); // kill <PID>

	setServerFd();
	if (getServerFd() == -1) {
		std::cerr << "Failed to create socket." << std::endl;
		return 1;
	}

	int	optVal = 1;
	if (setsockopt(getServerFd(), SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof(optVal)) < 0) {
		std::cerr << "setsockopt failed." << std::endl;
		close(getServerFd());
		return 1;
	}
	
	struct sockaddr_in serverAddress; // Struct to hold server address information
    memset(&serverAddress, 0, sizeof(serverAddress)); // Zero out the memory for the struct
    serverAddress.sin_family = AF_INET; // Set to IPv4
    serverAddress.sin_port = htons(getPort()); // Convert the port number to network byte order
    serverAddress.sin_addr.s_addr = INADDR_ANY; // Allow connections from any network interface

	if (bind(getServerFd(), (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
		std::cerr << "Bind failed." << std::endl;
		close(getServerFd());
		return 1;
	}

	if (listen(getServerFd(), MAX_CLIENTS) < 0) {
		std::cerr << "Listen failed." << std::endl;
		close(getServerFd());
		return 1;
	}

	std::cout << "Server listening on port: " << getPort() << std::endl;

	pollfd serverFd;
	serverFd.fd = getServerFd(); // Monitor the server socket for new connections
    serverFd.events = POLLIN; // POLLIN: Check for incoming data or connection requests
	serverFd.revents = 0; // Added revents to fill struct fields
    _fds.push_back(serverFd); // Add the server socket to the list of monitored sockets

	while (!_signal) { // while we don't recieve signal
		int pollCount = poll(_fds.data(), _fds.size(), -1);
		
		if (pollCount < 0) {
			if (errno == EINTR) {
			} else {
				std::cerr << "Poll failed" << std::endl;
			} 
			break;
		}
		
		for (size_t i = 0; i < _fds.size(); i++) {

			if (_fds[i].revents & POLLIN)
			{
				if (_fds[i].fd == getServerFd()) {
					acceptNewClient();
				}
				else {
					processClientInput(&i, _fds[i].fd);
				}
			}
		}
	}

	for (std::vector<pollfd>::iterator it = _fds.begin(); it != _fds.end(); ++it) {
		close(it->fd);
	}
	close(getServerFd());
	return 0;
}
