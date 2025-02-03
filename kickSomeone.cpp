#include "Server.hpp"

//Usage: KICK #channel nickname reason(reason is optional)

void Server::kickSomeone(int cfd, std::string arg) {
    // prints out for testing, delete it
    std::cout << "kickSomeone(): cfd is '" << cfd << "' and arg is '" << arg << "'." << std::endl;

    std::istringstream iss(arg); // converts this string to stream
    std::vector<std::string> tokens;
    std::string token;

    // splits the stream by delimiter. default delimiter is space
    while (iss >> token) {
        tokens.push_back(token);
    }

    std::string targetNick = tokens[2];
    std::string channelName = tokens[1];
    std::string reason;

    Client* executorClient = getClientObjByFd(cfd);
    if (!executorClient) {
        std::cerr << "executorClient '" << cfd << "' does not exist." << std::endl;
        return;
    }

    std::string executorNick = executorClient->getNick();

    if (tokens.size() < 3) {
        std::string errMsg = ":server 461 " + executorNick + " " + channelName + " :KICK :Not enough parameters\r\n";
        send(cfd, errMsg.c_str(), errMsg.length(), 0);
        std::cout << errMsg << std::endl; // for debugging
        return ;
    }

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
        std::string errMsg = ":server 403 " + executorNick + " " + channelName + " :No such channel\r\n";
        send(cfd, errMsg.c_str(), errMsg.length(), 0);
        std::cout << errMsg << std::endl; // for debugging, delete them
        return ;
    }

    int targetFd = getUserFdByNick(targetNick);
    if (targetFd == -1) {
        std::string errMsg = ":server 401 " + executorNick + " " + targetNick + " :No such nick/channel\r\n";
        send(cfd, errMsg.c_str(), errMsg.length(), 0);
        std::cout << errMsg << std::endl; // for debugging, delete this
        return ;
    }

    if (!isUserInChannel(channelName, targetFd)) {
        std::string errMsg = ":server 441 " + targetNick + " " + channelName + " :They aren't on that channel\r\n";
        send(cfd, errMsg.c_str(), errMsg.length(), 0);
        std::cout << errMsg << std::endl; // for debugging, delete them
        return ;
    }

    if (!hasOpRights(cfd, channelName)) {
        std::string errMsg = ":server 482 " + executorNick + " " + channelName + " :You're not channel operator\r\n";
        send(cfd, errMsg.c_str(), errMsg.length(), 0);
        std::cout << errMsg << std::endl; // for debugging, delete them
        return ;
    }

    Channel* channel = getChannelObj(channelName);
    if (!channel) {
        std::cerr << "Channel '" << channelName << "' does not exist." << std::endl;
        return;
    }

    Client* targetClient = getClientObjByFd(targetFd);
    if (!targetClient) {
        std::cerr << "targetClient '" << targetFd << "' does not exist." << std::endl;
        return;
    }

    channel->removeClient(targetFd);

    std::string targetUsername = targetClient->getUser();

    std::string kickAnnounce = ":" + executorNick + "!~" + executorClient->getUser() + "@" + executorClient->getIPa() 
    + " KICK " + channel->getChannelName() + " " + targetClient->getNick() 
    + " :" + (reason.empty() ? "" : reason) + "\r\n";

    send(targetFd, kickAnnounce.c_str(), kickAnnounce.length(), 0);
    channel->broadcast(kickAnnounce, cfd, false); // true or false, think about it again


    // for debugging
    if (isUserInChannel(channelName, targetFd)) {
    std::cout << "ERROR: " << targetNick << " is still in the channel " << channelName << " even after removal!" << std::endl;
    } else {
    std::cout << "SUCCESS: " << targetNick << " was successfully removed from " << channelName << "." << std::endl;
    }
    //

}
    