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
    std::string channelName = tokens[2];

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
        std::cout << "There is no such channel" << std::endl; // change the error message
        return ; // should I do something before return?
    }

    // does the target user exist?
    if (!nicknameExist(nickName)) {
        std::cout << "There is no such nickname" << std::endl; // change the error message
        return ; // should I do something before return?
    }

    if (!isUserInChannel(nickName, channelName)) {
        std::cout << "User is not in the channel" << std::endl; // change the error message
        return ; // should I do something before return?
    }


    // add the user to the invited list in channel class
}