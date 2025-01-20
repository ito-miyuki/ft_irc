#include <iostream>
#include "Server.hpp"
#include <exception>

/*
    0 ～ 1023: is already reserved for standard services(e.g., SSH, HTTP, HTTPS)
            it require administrative privileges to use.
    
    1024 - 49151: registered ports

    49152 ～ 65535: dynamic/private ports.
            Typically used by operating systems for short-lived client connections, leading to potential conflicts.
*/
bool isPortValid(const std::string& port) {

    if (port.empty()) {
        return false;
    }

    // only for over cpp+11
    for (char c : port) {
        if (!std::isdigit(c)) {
            std::cerr << "Port cannot contain non digit character" << std::endl; // you may delete it 
            return false;
        }
    }

    int portNum;
    try {
        portNum = std::stoi(port);
    } catch (const std::exception& e) {
        std::cerr << "Invalid port, from portNum: " << e.what() << std::endl; // change the message 
		return false;
    }
    
    if (portNum < 0 || portNum > 65535){
        std::cerr << "Port number is out of ranges" << std::endl; // you may delete it 
        return false;
    }
    return true;
}

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Wrong argument. Usage: " << argv[0] << " <port> <password>" << std::endl;
        return 1;
    }

    if (!isPortValid(argv[1])) {
        std::cerr << "Port '" << argv[1] << "' is invalid." << std::endl;
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