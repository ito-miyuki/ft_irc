// reference: https://www.geeksforgeeks.org/socket-programming-in-cpp/


#include "Server.hpp"
#include <sys/socket.h> // for socket()
#include <netinet/in.h> // for sockaddr_in
#include <unistd.h> // for close()
#include <iostream>
#include <cstring> // for memset
#include <vector> // for vector
#include <poll.h> // for pol()

#define MAX_CLIENTS 100

int Server::runServer() {

	// Step 1 : create server socket
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1) {
		std::cerr << "Failed to create socket." << std::endl;
		return 1;
	}
	/*
		socket() will return a file descriptor(fd),
		so "serverSocket" = fd

		AF_INET: It specifies the IPv4 protocol family.

		SOCK_STREAM: It defines that the TCP type socket
		-> TCP type: this type make sure the connection is fine before starting
			while UDP don't.
	*/

	// Step 2: define server address
	sockaddr_in serverAddress; // struct to store the address of the socket.
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET; // set IPv4
	serverAddress.sin_port = htons(_port); //to convert the port numbers from machine byte order to network byte order.
	serverAddress.sin_addr.s_addr = INADDR_ANY; // It accepts any connections from any IP address

	/*
		sin_*** are members of the sockaddr_in struct.
    	htons(): Converts the port number from host byte order
             to network byte order (big-endian).
    	INADDR_ANY: Allows the server to accept connections from
			all network interfaces(e.g., localhost, LAN, etc.).
	*/

	// Step 3: Bind server socket (bind sokect and IP address)
	if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
		std::cerr << "Bind failed." << std::endl;
		close(serverSocket);
		return 1;
	}

	/*
		Binds the socket to the specified IP address and port.

		bind() parameters:
		1. socket_fd,
		2. pointer to the struct that has socket address,
		3. size of address (address length)
	*/

	// Step 4: Listen for Connection
	if (listen(serverSocket, 5) < 0) {
		std::cerr << "Listen failed." << std::endl;
		close(serverSocket);
		return 1;
	}

	/*
		listen() parameters:
		1. server socket.
		2. maximum number of queued connections (backlog).

		The second parameter defines how many connection requests can be queued
    	before they are accepted.
		If the queue is full, new connections will be refused.
	*/

	std::cout << "Server listening on port: " << getPort() << std::endl;

	// Step 5: set up Poll (Poll monitors/tracks multiple sockets)
	std::vector<pollfd> fds; //pullfd is a struct
	pollfd serverFd;
	serverFd.fd = serverSocket; // serverSocket is waiting for client。
	serverFd.events = POLLIN; // // Monitor for incoming connections
	fds.push_back(serverFd);
	/*
		struct pollfd {
			int fd;
			short events; // event to tracl (例: POLLIN)
			short revents;// event that actually happned (poll()で設定される)
		};

	*/

	while (true) {
		// step 6: use poll to monitor sockets
		int pollCount = poll(fds.data(), fds.size(), -1);
		if (pollCount < 0) {
			std::cerr << "Poll failed" << std::endl;
			break;
		}

		/*
			poll() waits indefinitely (-1 timeout) for events on the monitored sockets.
			If a timeout (e.g., 5000ms) is specified, it will return after the timeout
			even if no events occur. Useful for handling periodic tasks in real applications.
		*/


		for (size_t i = 0; i < fds.size(); ++i) {
			//  step 7: Check if server socket is ready to accept new connection
			if (fds[i].fd == serverSocket && (fds[i].revents & POLLIN)) {
				int clientSocket = accept(serverSocket, nullptr, nullptr);
				if (clientSocket < 0) {
					std::cerr << "Accept failed" << std::endl;
					continue ;
				}
				std::cout << "New client connected: " << clientSocket << std::endl;
				/*
					accept() return fd, when it's error, return -1.
					get connection request from the que and accept it.
					accept() creates a new socket for the specific client connection.
					To handle multiple clients, we use non-blocking mode or pull()

					accept() parameters:
					1. server socket,
					2. struct that has client information (or nullptr),
					3. client address length(or nullptr)
					->if we don't need cient info or needs to output a log, use nullptr.
				*/

				// Add new client to pollfd list
				pollfd clientFd;
				clientFd.fd = clientSocket;
				clientFd.events = POLLIN;
				fds.push_back(clientFd);

			//step 8: check for client data
			} else if (fds[i].revents & POLLIN) {
				char buffer[1024] = {0};
				int byteRead = recv(fds[i].fd, buffer, sizeof(buffer), 0);
				/*
					recv() is for recieving data from client.
					it returns number of bytes. 0 = client disconnected, -1 = error

					recv() parameters:
					1. socket fd that is usually the one accept() returned,
					2. buffer,
					3. buffer size,
					4. flags, it's usually 0
				*/

				if (byteRead < 0) { // <= or < ?
					//client disconnected
					std::cout << "Client disconnected: " << fds[i].fd << std::endl;
					close(fds[i].fd);
					fds.erase(fds.begin() + i);
					--i;

					/*
						fds.erase() removes the socket from the vector and automatically
						shifts subsequent elements to maintain a contiguous array.
					*/

				} else {
					// handle command here!
					std::string command(buffer, byteRead); // this is std::string's constructor
					command.erase(command.find_last_not_of("\r\n") + 1); // remove last \n, \r is ascii13, I might not need it
					std::cout << "command is: '" << command << "'" << std::endl; // just for testing, delete it!
					
					// add more validation for command(check empty, etc)!!! it can be done inside of handleCommand???
					
					// just for testing
					if (command == "NICK") {
						// handleNickCommand(fds[i].fd, command);
						// _clients[i]._nickname = 
						std::cout << "You're in NICK condition" << std::endl;
					}
					else if (command == "USER") {
						std::cout << "You're in USER condition" << std::endl;
					}

					// std::cout << "Message from cliens " << fds[i].fd << ": " << buffer << std::endl;
					send(fds[i].fd, buffer, byteRead, 0);
				}
			}
		}
	}

	for (std::vector<pollfd>::iterator it = fds.begin(); it != fds.end(); ++it) {
		close(it->fd);
	}

	/*
		Close all sockets and release resources to prevent memory leaks
		or dangling file descriptors after the server shuts down.
	*/

	close(serverSocket);

	return 0;
}