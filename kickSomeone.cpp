#include "Server.hpp"
#include <algorithm> // std::find_if are we allowed to use?

bool Server::channelExist(const std::string& channelName) {
    std::cout << "You are in channelExist()" << std::endl;
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

bool Server::userExist(const std::string& userName) {
    for (std::vector<Client>::iterator ite = _clients.begin(); ite != _clients.end(); ++ite) {
        if (ite->getUser() == userName) {
            std::cout << "Username exist" << std::endl; // do something
            return true;
        }
    }
    return false;
}

int Server::getUserFd(const std::string& userName) {
    for (std::vector<Client>::iterator ite = _clients.begin(); ite != _clients.end(); ++ite) {
        if (ite->getUser() == userName) {
            return ite->getFd();
        }
    }
    return -1;
}

bool Server::isUserInChannel(const std::string& userName, const std::string& channelName) {

    int targetFd = getUserFd(userName);
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
            std::cout << "The user '" << userName << "' is not in the channel '" << channelName << "'." << std::endl;
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

    // prints out for testing, delete it
    std::cout << "kickSomeone(): cdf is '" << cdf << "' and arg is '" << arg << "'." << std::endl;

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
        channelName = tokens[1].erase(0, 1); // do we need it?
    } else {
        channelName = tokens[1];
    }

    std::string target = tokens[2];

    std::string reason; // reason could be at index 3 or 3 + rest of tokens.
    bool foundColon = false;

    for (size_t i = 3; i < tokens.size(); i++) {
        if (!foundColon) {
            if (tokens[i][0] == ':') {
                foundColon = true;
                reason = tokens[i].substr(1);
            }
        } 
        else {
            reason += " " + tokens[i];
        }
    }

    if (!foundColon && tokens.size() > 3) {
        reason = tokens[3];
    }

    // prints out for testing, delete them
    std::cout << "Command is: " << command << std::endl;
    std::cout << "Channel name is: " << channelName << std::endl;
    std::cout << "Target is: " << target << std::endl;
    std::cout << "Reason is: " << reason << std::endl;

    if (!channelExist(channelName)) {
        std::cout << "There is no such channel" << std::endl; // change the error message
        return ; // should I do something before return?
    }

    if (!userExist(target)) {
        std::cout << "There is no such username" << std::endl; // change the error message
        return ; // should I do something before return?
    }

    if (!isUserInChannel(target, channelName)) {
        std::cout << "User is not in the channel" << std::endl; // change the error message
        return ; // should I do something before return?
    }
}
    