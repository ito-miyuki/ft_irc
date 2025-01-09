/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mito <mito@student.hive.fi>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 16:06:31 by mito              #+#    #+#             */
/*   Updated: 2025/01/09 17:16:00 by mito             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <cstring> // strlen

int main() {

	//creat client socket
	int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == -1) {
		std::cerr << "Failed to create socket." << std::endl;
		return 1;
	}

	//define server address
	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(8080);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	// connecting to server
	if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
		std::cerr << "Failed to connect" << std::endl;
		close(clientSocket);
		return 1;
	}

	/*
		connect() parameters:
		1. socket fd
		2. struct that has server's IP address and port number
		3. size of the struct

		connect() send a connect request to a server
		it return -1 when it has error
	*/

	// send data to the server
	const char* message = "Hello, server!";
	if (send(clientSocket, message, strlen(message), 0) < 0) {
		std::cerr << "Failed to send message" << std::endl;
	} else {
		std::cout << "Message sent successfully!" << std::endl;
	}

	/*
		1. socket fd. Tipically the socket that you got from socket() or accept()
		2. address of the message
		3. length of message
		4. usual way to send
	*/

	close(clientSocket);

	return 0;
}
