#include "Server.hpp"

//:sataskin!~sataskin@194.136.126.51 TOPIC #topictester :cat
//what I get from irssi : TOPIC #channel :new topic
// std::ctime(&now);

void    Server::changeTopic(Channel &channel, int cfd, std::string newTopic)
{
    int clientIndex = getClientIndex(cfd);

    if (clientIndex > -1)
    {
        Client &sender = _clients.at(clientIndex);
        channel.setTopic(newTopic);
        std::string broadcastMessage = ":" + sender.getNick() + "!~" + sender.getUser() + "@" + sender.getIPa() + " TOPIC " + channel.getChannelName() + " :" + newTopic + "\r\n"; 
        channel.broadcast(broadcastMessage, cfd, true);
        std::time_t now = std::time(nullptr);
        channel.setTopicUpdated(now);
    }
    else
        std::cout << "sender doesnt exist?" << std::endl;
}

void    Server::topic(int cfd, std::string arg)
{
    std::string message;
    message = arg.substr(6, arg.size() - 6);
    
    std::string recipient;
    size_t split = message.find(':');
    recipient = message.substr(0, split - 1);
    std::string newTopic;
    newTopic = message.substr(split + 1, message.size() - split);

    int channelIndex = getChannelIndex(recipient);

    if (channelIndex > -1)
    {
        Channel &channel = _channels.at(channelIndex);
        std::vector<int> ops = channel.getOps();
        if (std::find(ops.begin(), ops.end(), cfd) != ops.end()) {
            changeTopic(channel, cfd, newTopic);
        } else if (channel.isTopicRestricted() == false && channel.containSender(cfd) == true) {
            changeTopic(channel, cfd, newTopic);
        } else {
            int clientIndex = getClientIndex(cfd);
            if (clientIndex > -1)
            {
                Client &sender = _clients.at(clientIndex);
                std::string sendThis;
                if (channel.containSender(cfd) == true)
                    sendThis = ":482" + sender.getNick() + "!~" + sender.getUser() + "@" + sender.getIPa() + " TOPIC " + recipient + " :You're not a channel operator\r\n";
                else
                    sendThis = ":442 " + sender.getNick() + "!~" + sender.getUser() + "@" + sender.getIPa() + " TOPIC " + recipient + " :You're not on that channel\r\n";
                send(cfd, sendThis.c_str(), sendThis.length(), 0);
                return ;
            }
        }
    }
    else {

        int clientIndex = getClientIndex(cfd);

        if (clientIndex > -1)
        {
            Client &sender = _clients.at(clientIndex);
            std::string sendThis = ":401 " + sender.getNick() + " " + recipient + " :No such channel\r\n";
            send(cfd, sendThis.c_str(), sendThis.length(), 0);
            return ;
        }
        
    }
}
