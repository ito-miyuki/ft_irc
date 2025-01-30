#include "Server.hpp"

/*message from irssi has this format
PRIVMSG <name or channel> :<the actual message we want to filter>
Channel name can only be gotten by checking if the first character 
is a hashtag or not*/

void    Server::sendToClient(int cfd, std::string dm, std::string recipient)
{
    Client  reci;
    Client  sender;
    
    if (checkClient(recipient, &reci) && checkSender(cfd, &sender)) {
        std::string sendThis = ":" + sender.getNick() + " PRIVMSG " + reci.getNick() + " :" + dm + "\r\n"; 
        send(reci.getFd(), sendThis.c_str(), sendThis.length(), 0);
    }
    else
        std::cout << "could not send to client" << std::endl;
}

void    Server::sendToChannel(int cfd, std::string dm, std::string recipient)
{
    Channel channel;
    (void)cfd;
    (void)dm;
    (void)recipient;
    if (checkChannel(recipient, &channel) == true)
        std::cout << "Will send to channel" << std::endl;
    else
        std::cout << "could not send to client" << std::endl;
}


void    Server::messages(int cfd, std::string arg) {
    std::cout << "This is what I got to function: " << arg << std::endl;
    
    std::string message;
    message = arg.substr(8, arg.size() - 8);
    std::cout << "This is after substr: " << message << std::endl;
    std::string recipient;
    size_t split = message.find(':');
    recipient = message.substr(0, split - 1);
    std::string dm;
    dm = message.substr(split + 1, message.size() - split);
    std::cout << "this is the recipient = " << recipient << "\n and this is the message = " << dm << std::endl;
    if (recipient[0] != '#')
        sendToClient(cfd, dm, recipient);
    else
        sendToChannel(cfd, dm, recipient);
}