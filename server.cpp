/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mito <mito@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 11:15:50 by mito              #+#    #+#             */
/*   Updated: 2025/01/09 16:44:59 by mito             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// reference: https://www.geeksforgeeks.org/socket-programming-in-cpp/

#include <sys/socket.h> // for socket()
#include <netinet/in.h> // for sockaddr_in
#include <unistd.h> // for close()
#include <iostream>


int main() {

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
	serverAddress.sin_family = AF_INET; // set IPv4
	serverAddress.sin_port = htons(8080); //to convert the port numbers from machine byte order to network byte order.
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

	// Step 5: Accept a client connection
	int clientSocket = accept(serverSocket, nullptr, nullptr);

	/*
		accept() return fd, when it's error, return -1.
		get connection request from the que and accept it.
		To handle multiple clients, we use non-blocking mode or pull()

		accept() parameters:
		1. server socket,
		2. struct that has client information (or nullptr),
		3. client address length(or nullptr)
		->if we don't need cient info or needs to output a log, use nullptr.
	*/

	// Step 6: Recive data from client
	char buffer[1024] = {0};
	recv(clientSocket, buffer, sizeof(buffer), 0);
	std::cout << "Message from client: " << buffer << std::endl;

	/*
		recv() is for recieving data from client.
		it returns number of bytes. 0 = client disconnected, -1 = error

		recv() parameters:
		1. socket fd that is usually the one accept() returned,
		2. buffer,
		3. buffer size,
		4. flags, it's usually 0
	*/

	close(clientSocket);

	return 0;
}
