#include "Server.hpp"

void Server::kickSomeone(int cdf, std::string arg) {
    std::istringstream iss(arg); // converts this string to stream
    std::vector<std::string> tokens;
    std::string token;


    // prints out for testing, delete them 
    std::cout << "Hello from kickSomeone function." << std::endl;
    std::cout << "cdf is '" << cdf << "' and arg is '" << arg << "'." << std::endl;

    // splits the stream by delimiter. default delimiter is space
    while (iss >> token) {
        tokens.push_back(token);
    }

    // prints out for testing, delete them
    for (const std::string& token : tokens) {
        std::cout << "Token: " << token << std::endl;
    }

    // we might not needed this becaus irss says ""
    if (tokens.size() < 3) {
        std::cerr << "At least 3 tokens needed to execute KICK" << std::endl;
    }

    // std::vector<std::string>::iterator end = tokens.end();
    // for (std::vector<std::string>::iterator begin = tokens.begin();  begin != end; ++begin) {
    //     if (begin != this->_clients.end())
    // }

    if (_channels.find(channelName) == _channels.end()) {
        std::cerr << "Channel '" << channelName << "' does not exist." << std::endl;
        return;
    }
    this->_channels


}