#include "Server.hpp"
#include <algorithm> // std::find_if are we allowed to use?

bool Server::isChannelExist(const std::string& channelName) {
    std::cout << "You are in isChannelExist()" << std::endl;
    std::cout << "Total channels in _channels: " << _channels.size() << std::endl;

    for (std::vector<Channel>::iterator ite = _channels.begin(); ite != _channels.end(); ++ite) {
        std::cout << "channel name that I am looking at is: " << ite->getChannelName() << std::endl;
        if (ite->getChannelName() == channelName) {
            std::cout << "Channel name exist" << std::endl; // do something
            return true;
        }
    }
    return false;
}

bool Server::isUserExist(const std::string& target) {
    for (std::vector<Client>::iterator ite = _clients.begin(); ite != _clients.end(); ++ite) {
        if (ite->getUser() == target) {
            std::cout << "Username exist" << std::endl; // do something
            return true;
        }
    }
    return false;
}

int Server::getTargetFd(const std::string& target) {
    for (std::vector<Client>::iterator ite = _clients.begin(); ite != _clients.end(); ++ite) {
        if (ite->getUser() == target) {
            return ite->getFd();
        }
    }
    return -1;
}

bool Server::isUserInChannel(const std::string& target, const std::string& channelName) {

    int targetFd = getTargetFd(target);
    if (targetFd == -1) {
        std::cout << "The user doesn't exist in the server" << std::endl; // change the message
        return false;
    }

    for (std::vector<Channel>::iterator ite = _channels.begin(); ite != _channels.end(); ++ite) {
        if (ite->getChannelName() == channelName) {
            std::vector<int> jointClients = ite->getJointClients();

            std::vector<int>::iterator found = std::find(jointClients.begin(), jointClients.end(), targetFd);
            if(found != jointClients.end()) {
                std::cout << "Username exist!" << std::endl; // do something
                return true;
            }
            std::cout << "The user '" << target << "' is not in the channel '" << channelName << "'." << std::endl;
            return false;
        }
    }
    std::cout << "The channel doesn't exist" << std::endl; // change the message
    return false;
}

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

    // assuming that parameter are in correct format /KICK #channel user name
    std::string command = tokens[0];
    std::string channelName;
    if (!tokens[1].empty() && tokens[1].at(0) == '#') {
        channelName = tokens[1].erase(0, 1);
    } else {
        channelName = tokens[1];
    }
    std::string target = tokens[2];
    std::string reason = tokens[3]; // reason could be 3 + rest of tokens.

    // prints out for testing, delete them
    for (const std::string& token : tokens) {
        std::cout << "Token: " << token << std::endl;
    }

    if (!isChannelExist(channelName)) {
        std::cout << "There is no such channel" << std::endl; // change the error message
        return ; // should I do something before return?
    }

    if (!isUserExist(target)) {
        std::cout << "There is no such username" << std::endl; // change the error message
        return ; // should I do something before return?
    }

    if (!isUserInChannel(target, channelName)) {
        std::cout << "User is not in the channel" << std::endl; // change the error message
        return ; // should I do something before return?
    }

}
    