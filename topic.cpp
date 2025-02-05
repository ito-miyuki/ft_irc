#include "Server.hpp"

void    Server::changeTopic(Channel &channel, int cfd, std::string newTopic)
{
    int clientIndex = getClientIndex(cfd);
    if (clientIndex > -1)
    {
        Client &sender = _clients.at(clientIndex);
        if (newTopic.empty() && !channel.getTopic().empty()) {
            std::string response1 = ":ft_irc 332 " + sender.getNick() + " " + channel.getChannelName() + " :" + channel.getTopic() + "\r\n";
            std::time_t hello = channel.getTopicUpdated();
            std::string response2 = ":ft_irc 333 " + sender.getNick() + " " + channel.getChannelName() + " " + channel.getEditer() + " " + std::ctime(&hello) + "\r\n";

            send(cfd, response1.c_str(), response1.length(), 0);
            send(cfd, response2.c_str(), response2.length(), 0);
        } else if (newTopic.empty()) {
            std::string response = ":ft_irc 331 " + sender.getNick() + " " + channel.getChannelName() + " :No topic is set\r\n";
            send(cfd, response.c_str(), response.length(), 0);
        } else {
            channel.setTopic(newTopic);
            std::string broadcastMessage = ":" + sender.getNick() + "!~" + sender.getUser() + "@" + sender.getIPa() + " TOPIC " + channel.getChannelName() + " :" + newTopic + "\r\n"; 
            channel.broadcast(broadcastMessage, cfd, true);
            std::time_t now = std::time(nullptr);
            channel.setEditer(sender.getNick());
            channel.setTopicUpdated(now);
        }
    }
}

void    Server::topic(int cfd, std::string arg)
{
    std::string cmnd;
    std::istringstream argument(arg);
    argument >> cmnd;
    
    std::string recipient;
    argument >> recipient;
    if (recipient.empty())
    {
        int clientIndex = getClientIndex(cfd);
        if (clientIndex > -1) {
            Client &sender = _clients.at(clientIndex);
            std::string sendThis = ":ft_irc 461 " + sender.getNick() + " TOPIC :Not enough parameters\r\n";
            send(cfd, sendThis.c_str(), sendThis.length(), 0);
            return ;
        }
    }

    std::string newTopic;
    std::getline(argument, newTopic);

    while (newTopic[0] == ':' || newTopic[0] == ' ')
        newTopic.erase(0, 1);
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
                    sendThis = ":ft_irc 482 " + sender.getNick() + " " + channel.getChannelName() + " :You're not a channel operator\r\n";
                else
                    sendThis = ":ft_irc 442 " + sender.getNick() + " " + channel.getChannelName() + " :You're not on that channel\r\n";
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
            std::string sendThis = ":ft_irc 401 " + sender.getNick() + " " + recipient + " :No such channel\r\n";
            send(cfd, sendThis.c_str(), sendThis.length(), 0);
            return ;
        }
        
    }
}
