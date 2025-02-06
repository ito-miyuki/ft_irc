#include "../Server.hpp"

/*
message from irssi has this format
PRIVMSG <name or channel> :<the actual message we want to filter>
*/

void    Server::sendToClient(int cfd, std::string dm, std::string recipient)
{
    Client  reci;
    Client  sender;
    std::string sendThis;
    
    if (checkClient(recipient, &reci) && checkSender(cfd, &sender)) {
        sendThis = ":" + sender.getNick() + "!~" +  sender.getUser() + "@" + sender.getIPa() + " PRIVMSG " + reci.getNick() + " :" + dm + "\r\n"; 
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
}


void    Server::messages(int cfd, std::string arg) {    
    int clientIndex = getClientIndex(cfd);
    if (clientIndex > -1)
    {
        Client &sender = _clients.at(clientIndex);
        std::istringstream input(arg);
        std::string cmnd; 
        input >> cmnd;

        std::string recipient;
        input >> recipient;
        if (recipient.empty()) {
            std::string sendThis = ":ft_irc 411 " + sender.getNick() + " :No recipient given\r\n";
            send(cfd, sendThis.c_str(), sendThis.length(), 0);
            return ;
        }
        std::string dm;
        std::getline(input, dm);
		while (dm[0] == ' ')
			dm.erase(0, 1);
        if (dm[0] == ':')
            dm.erase(0, 1);
        if (dm.empty()) {
            std::string sendThis = ":ft_irc 412 " + sender.getNick() + " :No text to send\r\n";
            send(cfd, sendThis.c_str(), sendThis.length(), 0);
            return ;  
        }
    
        if (recipient[0] != '#')
            sendToClient(cfd, dm, recipient);
        else
            sendToChannel(cfd, dm, recipient);
    }
}
