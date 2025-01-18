#include <iostream>
#include "Server.hpp"
#include <exception>

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Wrong argument. Usage: " << argv[0] << " <port> <password>" << std::endl;
        return 1;
    }

    int port;
    try {
        port = std::stoi(argv[1]); // add more specific error here
    } catch (const std::exception& e) {
        std::cerr << "Invalid port: " << e.what() << std::endl;
		return 1;
    }

    std::cout << "Port: " << port << std::endl; // for testing. delete it.

    std::string password = argv[2]; // check if argv[2] password is in valid format
    try {
        Server server(port, password);
        server.runServer();
    } catch (const std::exception& e) {
        std::cerr << "Server error: " << e.what() << std::endl;
		return 1;
    }
    return 0;
}