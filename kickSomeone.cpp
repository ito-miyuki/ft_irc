#include "Server.hpp"
#include <algorithm> // std::find_if are we allowed to use?

bool Server::channelExist(const std::string& channelName) {
    std::cout << "You are in channelExist()" << std::endl; // delete it 
    std::cout << "Total channels in _channels: " << _channels.size() << std::endl; // delete it

    for (std::vector<Channel>::iterator ite = _channels.begin(); ite != _channels.end(); ++ite) {
        std::cout << "channel name that I am looking at is: " << ite->getChannelName() << std::endl;
        if (ite->getChannelName() == channelName) {
            std::cout << "Channel name exist" << std::endl; // do something
            return true;
        }
    }
    return false;
}

int Server::getUserFdByNick(const std::string& nickName) {
    for (std::vector<Client>::iterator ite = _clients.begin(); ite != _clients.end(); ++ite) {
        if (ite->getNick() == nickName) {
            return ite->getFd();
        }
    }
    return -1;
}

bool Server::isUserInChannel(const std::string& userName, const std::string& channelName, int userFd) {

    for (std::vector<Channel>::iterator ite = _channels.begin(); ite != _channels.end(); ++ite) {
        if (ite->getChannelName() == channelName) {
            std::vector<int> jointClients = ite->getJointClients();

            std::vector<int>::iterator found = std::find(jointClients.begin(), jointClients.end(), userFd);
            if(found != jointClients.end()) {
                return true;
            }
            std::cout << "The user '" << userName << "' is not in the channel '" << channelName << "'." << std::endl;
            return false;
        }
    }
    std::cout << "The channel doesn't exist" << std::endl; // change the message
    return false;
}

//Usage: KICK #channel nickname reason(reason is optional)

void Server::kickSomeone(int cfd, std::string arg) {
    std::istringstream iss(arg); // converts this string to stream
    std::vector<std::string> tokens;
    std::string token;

    // prints out for testing, delete it
    std::cout << "kickSomeone(): cfd is '" << cfd << "' and arg is '" << arg << "'." << std::endl;

    // splits the stream by delimiter. default delimiter is space
    while (iss >> token) {
        tokens.push_back(token);
    }

    std::string targetNick = tokens[2];
    std::string channelName;
    std::string reason;

    if (!tokens[1].empty() && tokens[1].at(0) == '#') {
        channelName = tokens[1].erase(0, 1); // do we need it?
    } else {
        channelName = tokens[1];
    }

    // if there are not enough params -> should I use empty()?
    if (tokens.size() < 3) {
        std::string errMsg = ":server 461 " + targetNick + " " + "#" + channelName + " :KICK :Not enough parametersl\r\n";
        send(cfd, errMsg.c_str(), errMsg.length(), 0);
        std::cout << errMsg << std::endl; // for debugging
        return ;
    }

    // assuming that parameter are in correct format /KICK #channel user name

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
    std::cout << "channelName is: " << channelName << std::endl;
    std::cout << "targetNick is: " << targetNick << std::endl;
    std::cout << "reason is: " << reason << std::endl;

    if (!channelExist(channelName)) {
        std::string errMsg = ":server 403 " + targetNick + " " + "#" + channelName + " :No such channel\r\n";
        send(cfd, errMsg.c_str(), errMsg.length(), 0);
        std::cout << errMsg << std::endl; // for debugging, delete them
        return ;
    }

    int targetFd = getUserFdByNick(targetNick);
    if (targetFd == -1) {
        // should I send error to the server?
        std::cout << "There is no such username" << std::endl; // for debugging, delete them
        return ;
    }

    if (!isUserInChannel(targetNick, channelName, targetFd)) {
        std::string errMsg = ":server 441 " + targetNick + " " + "#" + channelName + " :They aren't on that channel\r\n";
        send(cfd, errMsg.c_str(), errMsg.length(), 0);
        std::cout << errMsg << std::endl; // for debugging, delete them
        return ;
    }

    Channel* channel = getChannelObj(channelName);
    if (!channel) {
        std::cerr << "Channel '" << channelName << "' does not exist." << std::endl;
        return;
    }

    Client* client = getClientObjByFd(targetFd);
    if (!client) {
        std::cerr << "Client '" << targetFd << "' does not exist." << std::endl;
        return;
    }

    channel->removeClient(targetFd);
    std::string targetUsername = client->getUser();
    std::string kickAnnounce = ":" + targetNick + "!" + targetUsername + "@localhost KICK " 
    + channelName + " " + targetNick + " :" + reason + "\r\n";
    channel->broadcast(kickAnnounce, cfd, true); // false?
}
    