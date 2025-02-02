#include "Server.hpp"

bool	Server::isClient(std::string nick)
{
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); std::advance(it, 1))
	{
		if ((*it).getNick() == nick)
			return (true);
	}
	return (false);
}

bool	Server::verifyParams(int cfd, std::vector<std::string> &params)
{
	int	chIndex = getChannelIndex(params.at(0));

	if (chIndex < 0)
	{
		if (!isClient(params.at(0)))
		{
			std::string msg = ":ircserv 403 " + _clients.at(getClientIndex(cfd)).getUser() + " " + params.at(0) + " :No such channel\r\n";
			send(cfd, msg.c_str(), msg.length(), 0);
			return (false);
		}
		return (true);
	}
	if (params.size() > 1 && params.at(1) != "+i" && params.at(1) != "-i" && params.at(1) != "+t" && params.at(1) != "-t"
		&& params.at(1) != "+k" && params.at(1) != "-k" && params.at(1) != "+o" && params.at(1) != "-o"
		&& params.at(1) != "+l" && params.at(1) != "-l")
		return (false);
	return (true);
}

void	Server::setInviteStatus(int cfd, Channel &channel, std::string mode)
{
	if (mode.front() == '+' && !channel.isInviteOnly())
	{
		channel.setInviteOnly(true);
		std::string msg = ":" + _clients.at(getClientIndex(cfd)).getNick() + "!" + _clients.at(getClientIndex(cfd)).getUser() + "@" + _clients.at(getClientIndex(cfd)).getIPa() + " MODE " + channel.getChannelName() + " " + mode + "\r\n";
		channel.broadcast(msg, cfd, true);
	}
	else if (mode.front() == '-' && channel.isInviteOnly())
	{
		channel.setInviteOnly(false);
		std::string msg = ":" + _clients.at(getClientIndex(cfd)).getNick() + "!" + _clients.at(getClientIndex(cfd)).getUser() + "@" + _clients.at(getClientIndex(cfd)).getIPa() + " MODE " + channel.getChannelName() + " " + mode + "\r\n";
		channel.broadcast(msg, cfd, true);
	}
}

void	Server::setTopicRestriction(int cfd, Channel &channel, std::string mode)
{
	if (mode.front() == '+' && !channel.isTopicRestricted())
	{
		channel.setTopicRestricted(true);
		std::string msg = ":" + _clients.at(getClientIndex(cfd)).getNick() + "!" + _clients.at(getClientIndex(cfd)).getUser() + "@" + _clients.at(getClientIndex(cfd)).getIPa() + " MODE " + channel.getChannelName() + " " + mode + "\r\n";
		channel.broadcast(msg, cfd, true);
	}
	else if (mode.front() == '-' && channel.isTopicRestricted())
	{
		channel.setTopicRestricted(false);
		std::string msg = ":" + _clients.at(getClientIndex(cfd)).getNick() + "!" + _clients.at(getClientIndex(cfd)).getUser() + "@" + _clients.at(getClientIndex(cfd)).getIPa() + " MODE " + channel.getChannelName() + " " + mode + "\r\n";
		channel.broadcast(msg, cfd, true);
	}
}

void	Server::setKey(int cfd, Channel &channel, std::vector<std::string> &params)
{
	if (params.at(1).front() == '+')
	{
		if (params.size() < 3)
		{
			std::string msg = ":ircserv 461 " + _clients.at(getClientIndex(cfd)).getUser() + " MODE :Not enough parameters\r\n";
			send(cfd, msg.c_str(), msg.length(), 0);
			return ;
		}
		channel.setKey(params.at(2));
		std::string msg = ":" + _clients.at(getClientIndex(cfd)).getNick() + "!" + _clients.at(getClientIndex(cfd)).getUser() + "@" + _clients.at(getClientIndex(cfd)).getIPa() + " MODE " + channel.getChannelName() + " " + params.at(1) + " " + params.at(2) + "\r\n";
		channel.broadcast(msg, cfd, true);
	}
	else if (params.at(1).front() == '-')
	{
		channel.setKey("");
		std::string msg = ":" + _clients.at(getClientIndex(cfd)).getNick() + "!" + _clients.at(getClientIndex(cfd)).getUser() + "@" + _clients.at(getClientIndex(cfd)).getIPa() + " MODE " + channel.getChannelName() + " " + params.at(1) + "\r\n";
		channel.broadcast(msg, cfd, true);
	}
}

void	Server::setClientLimit(int cfd, Channel &channel, std::vector<std::string> &params)
{
	if (params.at(1).front() == '-' && params.size() < 3)
	{
		channel.setClientLimit(-1);
		std::string msg = ":" + _clients.at(getClientIndex(cfd)).getNick() + "!" + _clients.at(getClientIndex(cfd)).getUser() + "@" + _clients.at(getClientIndex(cfd)).getIPa() + " MODE " + channel.getChannelName() + " " + params.at(1) + "\r\n";
		channel.broadcast(msg, cfd, true);
	}
	else if (params.at(1).front() == '+')
	{
		if (params.size() < 3)
		{
			std::string msg = ":ircserv 461 " + _clients.at(getClientIndex(cfd)).getUser() + " MODE :Not enough parameters\r\n";
			send(cfd, msg.c_str(), msg.length(), 0);
			return ;
		}
		int	limit;
		try
		{
			limit = std::stoi(params.at(2));
		}
		catch (const std::exception& e)
		{
			return ;
		}
		if (limit < 0)
		{
			return ;
		}
		channel.setClientLimit(limit);
		std::string msg = ":" + _clients.at(getClientIndex(cfd)).getNick() + "!" + _clients.at(getClientIndex(cfd)).getUser() + "@" + _clients.at(getClientIndex(cfd)).getIPa() + " MODE " + channel.getChannelName() + " " + params.at(1) + " " + params.at(2) + "\r\n";
		channel.broadcast(msg, cfd, true);
	}
	
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

bool	Server::hasOpRights(int cfd, std::string channelName){

	int	chIndex = getChannelIndex(channelName);

	if (chIndex > -1){

		std::vector<int>	ops = _channels.at(chIndex).getOps();
		std::vector<int>::iterator result = std::find(ops.begin(), ops.end(), cfd);
		if (result != ops.end())
			return (true);
	}
	else if (isClient(channelName))
	{
		std::vector<Channel*>	&opChannels = _clients.at(getClientIndex(cfd)).getOpChannels();
		Client	target;

		getClient(channelName, &target);
		std::vector<Channel*>	&jointChannels = target.getJointChannels();

		for (std::vector<Channel*>::iterator it = opChannels.begin(); it != opChannels.end(); std::advance(it, 1))
		{
			for (std::vector<Channel*>::iterator ite = jointChannels.begin(); ite != jointChannels.end(); std::advance(it, 1))
			{
				if (*it == *ite)
					return (true);
			}
		}
	}
	return (false);
}

Channel	*Server::findChannel(Client &op, Client &newOp)
{
	std::vector<Channel*>	opChannels = op.getOpChannels();
	std::vector<Channel*>	jointChannels = newOp.getJointChannels();

	for (std::vector<Channel*>::iterator it = opChannels.begin(); it != opChannels.end(); std::advance(it, 1))
	{
		for (std::vector<Channel*>::iterator ite = jointChannels.begin(); ite != jointChannels.end(); std::advance(it, 1))
		{
			if (*it == *ite)
				return (*it);
		}
	}
	return (nullptr);
}

void	Server::setOpRights(int cfd, std::vector<std::string> &params)
{
	Client	target;

	getClient(params.at(0), &target);
	Channel	*opChannel = findChannel(_clients.at(getClientIndex(cfd)), target);
	if (opChannel)
	{
		if (params.at(1).front() == '-')
		{
			opChannel->removeOp(target.getFd());
			opChannel->addClient(target.getFd());
			target.removeOpChannel(opChannel);
			target.addChannel(opChannel);
		}
		else if (params.at(1).front() == '+')
		{
			opChannel->removeClient(target.getFd());
			opChannel->addOp(target.getFd());
			target.removeChannel(opChannel);
			target.addOpChannel(opChannel);
		}
	}
	// do we need an error here?
}

void	Server::setMode(int cfd, std::vector<std::string> &params)
{
	if (verifyParams(cfd, params))
	{
		if (hasOpRights(cfd, params.at(0)))
		{
			if (params.size() > 1 && !params.at(1).empty())
			{
				int	chIndex = getChannelIndex(params.at(0));
				if (params.at(1).back() == 'o' && chIndex < 0)
					setOpRights(cfd, params);
				else
				{
					Channel &channel = _channels.at(chIndex);
					if (params.at(1).back() == 'i' && params.size() < 3)
						setInviteStatus(cfd, channel, params.at(1));
					else if (params.at(1).back() == 't' && params.size() < 3)
						setTopicRestriction(cfd, channel, params.at(1));
					else if (params.at(1).back() == 'k')
						setKey(cfd, channel, params);
					else if (params.at(1).back() == 'l')
						setClientLimit(cfd, channel, params);
				}
			}
		}
		/*
		if (params.at(0) != _clients.at(getClientIndex(cfd)).getNick())
		{
			std::string	msg = ":ircserv 482 " + _clients.at(getClientIndex(cfd)).getUser() + " " + params.at(0) + " :You're not channel operator\r\n";
			send(cfd, msg.c_str(), msg.length(), 0);
		} */
	}
}

void	Server::mode(int cfd, std::string arg)
{
	std::vector<std::string>	params;

	parser(arg, params);
	if (params.empty())
	{
		std::string msg = ":ircserv 461 " + _clients.at(getClientIndex(cfd)).getUser() + " MODE :Not enough parameters\r\n";
		send(cfd, msg.c_str(), msg.length(), 0);
		return ;
	}
	setMode(cfd, params);
}
