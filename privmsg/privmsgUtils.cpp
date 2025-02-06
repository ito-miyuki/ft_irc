#include "../Server.hpp"

bool    Server::checkClient(std::string name, Client *client)
{
    std::vector<Client>::iterator end = _clients.end();
    for (std::vector<Client>::iterator it = _clients.begin(); it != end; std::advance(it, 1))
    {
        if (it->getNick() == name)
        {
            *client = *it;
            return (true);
        }
    }
    return (false);
}

bool    Server::checkChannel(std::string name, Channel *channel)
{
    std::vector<Channel>::iterator end = _channels.end();
    for (std::vector<Channel>::iterator it = _channels.begin(); it != end; std::advance(it, 1))
    {
        if (it->getChannelName() == name)
        {
            *channel = *it;
            return (true);
        }
    }
    return (false);
}

bool    Server::checkSender(int cfd, Client *client)
{
    std::vector<Client>::iterator end = _clients.end();
    for (std::vector<Client>::iterator it = _clients.begin(); it != end; std::advance(it, 1))
    {
        if (it->getFd() == cfd)
        {
            *client = *it;
            return (true);
        }
    }
    return (false);
}