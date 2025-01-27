#include "Server.hpp"

void Server::kickSomeone(int cdf, std::string arg) {
    std::istringstream iss(arg); // converts this string to stream
    std::vector<std::string> tokens;
    std::string token;

    std::cout << "Hello from kickSomeone function." << std::endl;
    std::cout << "cdf is '" << cdf << "' and arg is '" << arg << "'." << std::endl;

    // splits the stream by delimiter. default delimiter is space
    while (iss >> token) {
        tokens.push_back(token);
    }

    for (const std::string& token : tokens) {
        std::cout << "Token: " << token << std::endl;
    }
}