#include "../Server.hpp"

bool	Server::isClient(std::string nick)
{
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); std::advance(it, 1))
	{
		if ((*it).getNick() == nick)
			return (true);
	}
	return (false);
}

bool    Server::getClient(std::string name, Client *client)
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

std::string	Server::findChannel(Client &op, Client &newOp) {

	std::vector<std::string>	result;
	std::vector<std::string>	opChannels = op.getOpChannels();
	std::vector<std::string>	jointChannels = newOp.getJointChannels();

	std::sort(opChannels.begin(), opChannels.end());
	std::sort(jointChannels.begin(), jointChannels.end());

	std::set_intersection(opChannels.begin(), opChannels.end(), jointChannels.begin(), jointChannels.end(), std::back_inserter(result));
	if (!result.empty())
		return (result.at(0));
	return ("");
}

bool	Server::hasOpRights(int cfd, std::string channelName){

	int							chIndex = getChannelIndex(channelName);
	std::vector<int>			&ops = _channels.at(chIndex).getOps();
	std::vector<int>::iterator	result = std::find(ops.begin(), ops.end(), cfd);

	if (result != ops.end())
		return (true);
	return (false);
}

std::string	Server::findCommonChannel(int cfd, std::string targetNick) {

	int	targetFd = getUserFdByNick(targetNick);

	for (std::vector<Channel>::iterator it = _channels.begin(); it != _channels.end(); std::advance(it, 1)) {
		if ((it)->containSender(cfd) && (it)->containSender(targetFd))
			return (it->getChannelName());
	}
	return ("");
}
