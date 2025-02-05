#include "Server.hpp"

//Usage: KICK #channel nickname reason(reason is optional)

void Server::kickSomeone(int cfd, std::string arg) {

    std::istringstream iss(arg); // converts this string to stream
    std::vector<std::string> tokens;
    std::string token;

    // splits the stream by delimiter. default delimiter is space
    while (iss >> token) {
        tokens.push_back(token);
    }

    Client* executorClient = getClientObjByFd(cfd);
    if (!executorClient) {
        return;
    }

    std::string executorNick = executorClient->getNick();

    if (tokens.size() < 3) {
        std::string errMsg = ":ft_irc 461 " + executorNick + " :KICK :Not enough parameters\r\n";
        send(cfd, errMsg.c_str(), errMsg.length(), 0);
        return ;
    }

    std::string targetNick = tokens[2];
    std::string channelName = tokens[1];
    std::string reason;

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

    if (!channelExist(channelName)) {
        std::string errMsg = ":ft_irc 403 " + executorNick + " " + channelName + " :No such channel\r\n";
        send(cfd, errMsg.c_str(), errMsg.length(), 0);
        return ;
    }

    int targetFd = getUserFdByNick(targetNick);
    if (targetFd == -1) {
        std::string errMsg = ":ft_irc 401 " + executorNick + " " + targetNick + " :No such nick/channel\r\n";
        send(cfd, errMsg.c_str(), errMsg.length(), 0);
        return ;
    }

    if (!isUserInChannel(channelName, cfd)) {
        std::string errMsg = ":ft_irc 442 " + executorNick + " " + channelName + " :You're not on that channel\r\n";
        send(cfd, errMsg.c_str(), errMsg.length(), 0);
        return ;
    }

    if (!isUserInChannel(channelName, targetFd)) {
        std::string errMsg = ":ft_irc 441 " + targetNick + " " + channelName + " :They aren't on that channel\r\n";
        send(cfd, errMsg.c_str(), errMsg.length(), 0);
        return ;
    }

    if (!hasOpRights(cfd, channelName)) {
        std::string errMsg = ":ft_irc 482 " + executorNick + " " + channelName + " :You're not a channel operator\r\n";
        send(cfd, errMsg.c_str(), errMsg.length(), 0);
        return ;
    }

    Channel* channel = getChannelObj(channelName);
    if (!channel) {
        return;
    }

    Client* targetClient = getClientObjByFd(targetFd);
    if (!targetClient) {
        return;
    }

    channel->removeClient(targetFd);

    std::string targetUsername = targetClient->getUser();

    std::string kickAnnounce = ":" + executorNick + "!~" + executorClient->getUser() + "@" + executorClient->getIPa() 
    + " KICK " + channel->getChannelName() + " " + targetClient->getNick() 
    + " :" + (reason.empty() ? targetNick : reason) + "\r\n";

    send(targetFd, kickAnnounce.c_str(), kickAnnounce.length(), 0);
    channel->broadcast(kickAnnounce, cfd, false);
}
    