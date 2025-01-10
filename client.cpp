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
#include <cstring> // memset

#define  PORT 8080

int main() {
    // Step 1: Create client socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        perror("Failed to create socket");
        return 1;
    }

    // Step 2: Define server address
    sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr) <= 0) {
        perror("Invalid address");
        close(clientSocket);
        return 1;
    }

    // Step 3: Connect to server
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Failed to connect");
        close(clientSocket);
        return 1;
    }
    std::cout << "Connected to server" << std::endl;

    // Step 4: Communication loop
    while (true) {
        std::string message;
        std::cout << "Enter message (or 'exit' to quit): ";
        std::getline(std::cin, message);

        if (message == "exit") {
            break;
        }

        // Send message to server
        if (send(clientSocket, message.c_str(), message.length(), 0) < 0) {
            perror("Failed to send message");
            break;
        }

        // Receive response from server
        char buffer[1024] = {0};
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
            std::cout << "Disconnected from server" << std::endl;
            break;
        }

        std::cout << "Server response: " << buffer << std::endl;
    }

    // Clean up
    close(clientSocket);

    return 0;
}

// for a single client
// int main() {

// 	//creat client socket
// 	int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
// 	if (clientSocket == -1) {
// 		std::cerr << "Failed to create socket." << std::endl;
// 		return 1;
// 	}

// 	//define server address
// 	sockaddr_in serverAddress;
// 	serverAddress.sin_family = AF_INET;
// 	serverAddress.sin_port = htons(8080);
// 	serverAddress.sin_addr.s_addr = INADDR_ANY;

// 	// connecting to server
// 	if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
// 		std::cerr << "Failed to connect" << std::endl;
// 		close(clientSocket);
// 		return 1;
// 	}

// 	/*
// 		connect() parameters:
// 		1. socket fd
// 		2. struct that has server's IP address and port number
// 		3. size of the struct

// 		connect() send a connect request to a server
// 		it return -1 when it has error
// 	*/

// 	// send data to the server
// 	const char* message = "Hello, server!";
// 	if (send(clientSocket, message, strlen(message), 0) < 0) {
// 		std::cerr << "Failed to send message" << std::endl;
// 	} else {
// 		std::cout << "Message sent successfully!" << std::endl;
// 	}

// 	/*
// 		1. socket fd. Tipically the socket that you got from socket() or accept()
// 		2. address of the message
// 		3. length of message
// 		4. usual way to send
// 	*/

// 	close(clientSocket);

// 	return 0;
// }
