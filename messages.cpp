#include "Server.hpp"

/*message from irssi has this format
PRIVMSG <name or channel> :<the actual message we want to filter>
Channel name can only be gotten by checking if the first character 
is a hashtag or not*/

void    Server::sendToClient(int cfd, std::string dm, std::string recipient)
{
    Client  reci;
    Client  sender;
    std::string sendThis;
    
    if (checkClient(recipient, &reci) && checkSender(cfd, &sender)) {
        sendThis = ":" + sender.getNick() + " PRIVMSG " + reci.getNick() + " :" + dm + "\r\n"; 
        send(reci.getFd(), sendThis.c_str(), sendThis.length(), 0);
    } else {
        sendThis = ":401 " + sender.getNick() + " " + recipient + " :No suck nick/channel\r\n";
        send(cfd, sendThis.c_str(), sendThis.length(), 0);
    }
}

void    Server::sendToChannel(int cfd, std::string dm, std::string recipient)
{
    Channel channel;
    Client  reci;
    Client  sender;
    std::string sendThis;

    if (checkSender(cfd, &sender)) {
        if (checkChannel(recipient, &channel) == true) {
            if (channel.containSender(cfd)) {
                sendThis = ":" + sender.getNick() + " #" + channel.getChannelName() + " :" + dm + "\r\n";
                std::vector<int> reci = channel.getJointClients();
                for (std::vector<int>::iterator it = reci.begin(); it != reci.end(); std::advance(it, 1))
                {
                    if (*it != cfd) {
                        send(*it, sendThis.c_str(), sendThis.length(), 0); 
                    }
                }
            } else {
                sendThis = ":404 " + sender.getNick() + " #" + channel.getChannelName() + " :Cannot send to channel\r\n";
                send(cfd, sendThis.c_str(), sendThis.length(), 0);
                return ;
            }
        } else {
            std::string sendThis = ":401 " + sender.getNick() + " " + recipient + " :No suck nick/channel\r\n";
            send(cfd, sendThis.c_str(), sendThis.length(), 0);
            return ;
        }
    }
}


void    Server::messages(int cfd, std::string arg) {
    std::cout << "This is what I got to function: " << arg << std::endl;
    
    std::string message;
    message = arg.substr(8, arg.size() - 8);
    std::string recipient;
    size_t split = message.find(':');
    recipient = message.substr(0, split - 1);
    std::string dm;
    dm = message.substr(split + 1, message.size() - split);
    if (recipient[0] != '#')
        sendToClient(cfd, dm, recipient);
    else
        sendToChannel(cfd, dm, recipient);
}