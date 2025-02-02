#include "Server.hpp"

// bool Server::nicknameExist(const std::string& nickname) {
//     for (std::vector<Client>::iterator ite = _clients.begin(); ite != _clients.end(); ++ite) {
//         if (ite->getNick() == nickname) {
//             std::cout << "nickname exist" << std::endl; // do something
//             return true;
//         }
//     }
//     return false;
// }

int Server::getUserFdbyNick(const std::string& nickName) {
    for (std::vector<Client>::iterator ite = _clients.begin(); ite != _clients.end(); ++ite) {
        if (ite->getNick() == nickName) {
            return ite->getFd();
        }
    }
    return -1;
}

/**
 * Invite command
 * Usage: INVITE <nickname> <channel>
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
        std::cerr << "executorClient '" << cfd << "' does not exist." << std::endl;
        return;
    }

    std::string targetNick = tokens[1];
    std::string channelName = tokens[2];

    std::string executorNick = executorClient->getNick();

    if (tokens.size() < 3) {
        std::string errMsg = ":server 461 " + executorNick + " " + channelName + " :KICK :Not enough parametersl\r\n";
        send(cfd, errMsg.c_str(), errMsg.length(), 0);
        std::cout << errMsg << std::endl; // for debugging
        return ;
    }

    // for testing print out, delete them
    std::cout << "target nickname is " << targetNick << std::endl;
    std::cout << "channelName is " << channelName << std::endl;
    std::cout << "executorNick is " << executorNick << std::endl;

    if (!channelExist(channelName)) {
        std::string errMsg = ":server 403 " + targetNick + " " + channelName + " :No such channel\r\n";
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

    if (isUserInChannel(targetNick, channelName, targetFd)) {
        std::string errMsg = ":server 443 " + targetNick + " " + channelName + " :is already on channel\r\n";
        send(cfd, errMsg.c_str(), errMsg.length(), 0);
        std::cout << errMsg << std::endl; // for debugging, delete them
        return ;
    }

    if (isUserInChannel(targetNick, channelName, cfd)) {
        std::string errMsg = ":server 442 " + executorNick + " " + channelName + " :You're not on that channel\r\n";
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
        std::cerr << "Channel '" << channelName << "' does not exist!" << std::endl;
        return;
    }

    // add the user to the invited list in channel class
    channel->addToInvitedClients(targetFd);

    std::string invitation = ":" + executorNick + "!~" + executorClient->getUser() + "@" + executorClient->getIPa() 
    + " INVITE " + targetNick + " :" + channel->getChannelName() + "\r\n";

    send(targetFd, invitation.c_str(), invitation.length(), 0);
    // do we want to broadcast??
    channel->broadcast(invitation, cfd, false); // true or false, think about it again
}