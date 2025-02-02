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
        sendThis = ":ft_irc 401 " + sender.getNick() + "!~" + sender.getUser() + sender.getIPa() + " " + recipient + " :No such nick/channel\r\n";
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
                sendThis = ":" + sender.getNick() + "!~" +  sender.getUser() + "@" + sender.getIPa() + " PRIVMSG " + channel.getChannelName() + " :" + dm + "\r\n";
                channel.broadcast(sendThis, cfd, false);
            } else {
                sendThis = ":ft_irc 404 " + sender.getNick() + " " + channel.getChannelName() + " :Cannot send to channel\r\n";
                send(cfd, sendThis.c_str(), sendThis.length(), 0);
                return ;
            }
        } else {
            std::string sendThis = ":ft_irc 401 " + sender.getNick() + " " + recipient + " :No such nick/channel\r\n";
            send(cfd, sendThis.c_str(), sendThis.length(), 0);
            return ;
        }
    }
    std::cerr << "How do you not exist?" << std::endl;
}


void    Server::messages(int cfd, std::string arg) {    
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
