#include "Server.hpp"

/**
 * Invite command
 * Usage: INVITE <nickname> <channel> or
 * Usage: INVITE <nickname>
*/

void Server::inviteRandos(int cfd, std::string arg){
    // delete it
    std::cout << "inviteRandos(): cfd is " << cfd << " and arg is " << arg << std::endl;

    std::istringstream iss(arg); // converts this string to stream
    std::vector<std::string> tokens;
    std::string token;

    // splits the stream by delimiter. default delimiter is space
    while (iss >> token) {
        tokens.push_back(token);
    }

    Client* executorClient = getClientObjByFd(cfd);
    if (!executorClient) {
        //std::cerr << "executorClient '" << cfd << "' does not exist." << std::endl;
        return;
    }

    std::string executorNick = executorClient->getNick();

    if (tokens.size() < 3) {
        std::string errMsg = ":ft_irc 461 " + executorNick + " :INVITE :Not enough parameters\r\n";
        send(cfd, errMsg.c_str(), errMsg.length(), 0);
        std::cout << errMsg << std::endl; // for debugging
        return ;
    }

    std::string targetNick = tokens[1];
    std::string channelName = tokens[2];


    if (!channelExist(channelName)) {
        std::string errMsg = ":ft_irc 403 " + executorNick + " " + channelName + " :No such channel\r\n";
        send(cfd, errMsg.c_str(), errMsg.length(), 0);
        std::cout << errMsg << std::endl; // for debugging, delete them
        return ;
    }

    int targetFd = getUserFdByNick(targetNick);
    if (targetFd == -1) {
        std::string errMsg = ":ft_irc 401 " + executorNick + " " + targetNick + " :No such nick/channel\r\n";
        send(cfd, errMsg.c_str(), errMsg.length(), 0);
        std::cout << errMsg << std::endl; // for debugging, delete this
        return ;
    }

    if (!isUserInChannel(channelName, cfd)) {
        std::string errMsg = ":ft_irc 442 " + executorNick + " " + channelName + " :You're not on that channel\r\n";
        send(cfd, errMsg.c_str(), errMsg.length(), 0);
        std::cout << errMsg << std::endl; // for debugging, delete them
        return ;
    }

    if (isUserInChannel(channelName, targetFd)) {
        std::string errMsg = ":ft_irc 443 " + executorNick + " " + targetNick + " " + channelName + " :is already on channel\r\n";
        send(cfd, errMsg.c_str(), errMsg.length(), 0);
        std::cout << errMsg << std::endl; // for debugging, delete them
        return ;
    }

    if (!hasOpRights(cfd, channelName)) {
        std::string errMsg = ":ft_irc 482 " + executorNick + " " + channelName + " :You're not a channel operator\r\n";
        send(cfd, errMsg.c_str(), errMsg.length(), 0);
        std::cout << errMsg << std::endl; // for debugging, delete them
        return ;
    }

    Channel* channel = getChannelObj(channelName);
    if (!channel) {
        std::cerr << "Channel '" << channelName << "' does not exist!" << std::endl;
        return;
    }

    channel->addToInvitedClients(targetFd);

    std::string invitation = ":" + executorNick + "!~" + executorClient->getUser() + "@" + executorClient->getIPa() 
    + " INVITE " + targetNick + " :" + channel->getChannelName() + "\r\n";
    send(targetFd, invitation.c_str(), invitation.length(), 0);

    std::string successInvite = ":ft_irc 341 " + executorNick + " " + targetNick + " " + channelName + "\r\n";
    send(cfd, successInvite.c_str(), successInvite.length(), 0);
}