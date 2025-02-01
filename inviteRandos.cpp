#include "Server.hpp"

bool Server::nicknameExist(const std::string& nickname) {
    for (std::vector<Client>::iterator ite = _clients.begin(); ite != _clients.end(); ++ite) {
        if (ite->getNick() == nickname) {
            std::cout << "nickname exist" << std::endl; // do something
            return true;
        }
    }
    return false;
}

int Server::getUserFdbyNick(const std::string& nickName) {
    for (std::vector<Client>::iterator ite = _clients.begin(); ite != _clients.end(); ++ite) {
        if (ite->getUser() == nickName) {
            return ite->getFd();
        }
    }
    return -1;
}

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

    // prints out for testing, delete them
    for (const std::string& token : tokens) {
        std::cout << "Token: " << token << std::endl;
    }

    std::string nickName = tokens[1];
    std::string channelName;
    if (!tokens[2].empty() && tokens[2].at(0) == '#') {
        channelName = tokens[2].erase(0, 1); // do we need it?
    } else {
        channelName = tokens[2];
    }

    std::cout << "nickname is " << nickName << std::endl;
    std::cout << "channelName is " << channelName << std::endl;

    //misisng validation here
    /*
        - am I in channel -> if not, ....?
        - am I an operator? -> you cannot execute
        - are the target in the channel? -> if yes, .....?
    
    */

   // - does Channel exist?
    if (!channelExist(channelName)) {
        std::string errMsg = ":server 403 " + nickName + " " + "#" + channelName + " :No such channel\r\n";
        send(cfd, errMsg.c_str(), errMsg.length(), 0);
        std::cout << errMsg << std::endl; // for debugging
        return ; // should I do something before return?
    }

    int userFd = getUserFdByNick(nickName);
    if (userFd == -1) {
        std::cout << "There is no such username" << std::endl; // for debugging
        return ;
    }

    // does the target user exist?
    if (!nicknameExist(nickName)) {
        std::cout << "There is no such nickname" << std::endl; // for debugging
        return ; // should I do something before return?
    }

    if (isUserInChannel(nickName, channelName, userFd)) {
        std::cout << "User is already in the channel" << std::endl; // for debugging
        return ; // should I do something before return?
    }

    Channel* channel = getChannelObj(channelName);
    if (!channel) {
        std::cerr << "Channel '" << channelName << "' does not exist!" << std::endl;
        return;
    } 

    // add the user to the invited list in channel class
    channel->addToInvitedClients(userFd);

}