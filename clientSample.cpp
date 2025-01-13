#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> // inet_addr
#include <iostream>
#include <unistd.h>
#include <cstring> // strlen, memset

#define  PORT 8080

int main() {
    // Step 1: Create client socket
    // The client socket will be used to communicate with the server.
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        perror("Failed to create socket");
        return 1;
    }

    // Step 2: Define server address
    // Set up the server address structure with IPv4 and the specified port.
    sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;        // Use IPv4
    serverAddress.sin_port = htons(PORT);     // Convert port to network byte order

    // Convert the server address (127.0.0.1) to binary form
    in_addr_t result = inet_addr("127.0.0.1");
    if (result == INADDR_NONE) {
        std::cerr << "Invalid address" << std::endl;
        return 1;
    }
    serverAddress.sin_addr.s_addr = result;

    // Step 3: Connect to server
    // Attempt to establish a connection to the server.
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Failed to connect");
        close(clientSocket);
        return 1;
    }
    std::cout << "Connected to server" << std::endl;

    // Step 4: Communication loop
    // The client will continuously send messages to the server until "exit" is entered.
    while (true) {
        std::string message;
        std::cout << "Enter message (or 'exit' to quit): ";
        std::getline(std::cin, message);

        if (message == "exit") {
            // Exit the communication loop
            break;
        }

        // Send message to the server
        if (send(clientSocket, message.c_str(), message.length(), 0) < 0) {
            perror("Failed to send message");
            break;
        }

        // Receive response from the server
        char buffer[1024] = {0}; // Buffer to store the response
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
            // Server has disconnected or an error occurred
            std::cout << "Disconnected from server" << std::endl;
            break;
        }

        // Print the server's response
        std::cout << "Server response: " << buffer << std::endl;
    }

    // Clean up
    // Close the client socket before exiting
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
