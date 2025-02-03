#include "Server.hpp"

//:sataskin!~sataskin@194.136.126.51 TOPIC #topictester :cat
//what I get from irssi : TOPIC #channel :new topic

//void    Server::changeTopic(Channel channel, int cfd, std::string newTopic)
//{

//}

void    Server::topic(int cfd, std::string arg)
{
    Channel channel;

    std::string message;
    message = arg.substr(6, arg.size() - 6);
    
    std::string recipient;
    size_t split = message.find(':');
    recipient = message.substr(0, split - 1);
    std::string newTopic;
    newTopic = message.substr(split + 1, message.size() - split);
    if (checkChannel(recipient, &channel) == true) {
        std::vector<int> ops = channel.getOps();
        if (std::find(ops.begin(), ops.end(), cfd) == ops.end()) {
            std::cout << "is an op and can change the topic" << std::endl;
        //    changeTopic(channel, cfd, newTopic);
        }
        else if (channel.isTopicRestricted() == false && channel.containSender(cfd) == true)
        {
            std::cout << "they can also change the topic" << std::endl;
         //   changeTopic(channel, cfd, newTopic);
        }
        else {
            std::cout << "error message. cant change the topic." << std::endl;
        }
    }
    else {
        Client sender;
        checkSender(cfd, &sender);
        std::string sendThis = ":401 " + sender.getNick() + " " + recipient + " :No such channel\r\n";
        send(cfd, sendThis.c_str(), sendThis.length(), 0);
        return ;
    }
}