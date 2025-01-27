#include "Server.hpp"
#include <algorithm> // std::find_if are we allowed to use?

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

    // we might not needed this because irssi don't accept it 
    if (tokens.size() < 3) {
        std::cerr << "At least 3 tokens needed to execute KICK" << std::endl;
        return ;
    }

    std::string command = tokens[0];
    std::string channelName = tokens[1];
    std::string target = tokens[2];
    std::string reason = tokens[3]; // reason could be 3 + rest of tokens.

    // assuming the correct value comes for now but add something 

    // add something to check if the channel exist

    // std::vector<std::string>::iterator ite = std::find(_channels.begin(), _channels.end(), channelName);
    // auto ite = std::find(_channels.begin(), _channels.end(), channelName);
    // if (ite != _channels.end()) {
    //     std::cout << "Channel name exist" << std::endl;
    // } else {
    //     std::cout << "There is no such channel" << std::endl;
    //     return ;
    // }

}
    