# include "Server.hpp"

bool	Server::isClient(std::string nick)
{
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); std::advance(it, 1))
	{
		if ((*it).getNick() == nick || (*it).getUser() == nick) // one of these might be unnecessary
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
		// do we need to inform the client about the channel modes...?
		return (false);
	return (true);
}

void	Server::setInviteStatus(int cfd, Channel &channel, std::string mode)
{
	if (mode.front() == '+' && !channel.isInviteOnly())
	{
		channel.setInviteOnly(true);
		//send info message?
	}
	else if (mode.front() == '-' && channel.isInviteOnly())
	{
		channel.setInviteOnly(false);
		//send info message?
	}
}

void	Server::setTopicRestriction(int cfd, Channel &channel, std::string mode)
{
	if (mode.front() == '+' && !channel.isTopicRestricted())
	{
		channel.setTopicRestricted(true);
		//send info message?
	}
	else if (mode.front() == '-' && channel.isTopicRestricted())
	{
		channel.setTopicRestricted(false);
		//send info message?
	}
}

void	Server::setKey(int cfd, Channel &channel, std::vector<std::string> &params)
{
	if (params.at(1).front() == '+')
	{
		if (params.size() < 3)
			//print error
		channel.setKey(params.at(2));
		//send info message?
	}
	else if (params.at(1).front() == '-' && params.size() < 3)
	{
		channel.setKey("");
		//send info message?
	}
}

void	Server::setClientLimit(int cfd, Channel &channel, std::vector<std::string> &params)
{
	if (params.at(1).front() == '-' && params.size() < 3)
	{
		channel.setClientLimit(-1);
		//send info message?
	}
	else if (params.at(1).front() == '+')
	{
		if (params.size() < 3)
		{
			// print error
		}
		int	limit;
		try
		{
			limit = std::stoi(params.at(2));
		}
		catch (const std::exception& e)
		{
			// print error
			return ;
		}
		if (limit < 0)
			//print error
		channel.setClientLimit(limit);
		//send info message?
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
		std::vector<Channel*>	opChannels = _clients.at(getClientIndex(cfd)).getOpChannels();
		Client	target;

		getClient(channelName, &target);
		std::vector<Channel*>	jointChannels = target.getJointChannels();

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

Channel	&Server::findChannel(Client &op, Client &newOp)
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

}

void	Server::setOpRights(int cfd, std::vector<std::string> &params)
{
	Client	target;

	getClient(params.at(0), &target);
	Channel	opChannel = findChannel(_clients.at(getClientIndex(cfd)), target);
}

void	Server::setMode(int cfd, std::vector<std::string> &params)
{
	if (verifyParams(cfd, params))
	{
		if (hasOpRights(cfd, params.at(0)))
		{
			if (!params.at(1).empty())
			{
				int	chIndex = getChannelIndex(params.at(0));
				if (params.at(1).back() == 'o' && chIndex < 0)
					setOpRights(cfd, params);
				else
				{
					Channel channel = _channels.at(chIndex);
					if (params.at(1).back() == 'i' && params.empty())
						setInviteStatus(cfd, channel, params.at(1));
					else if (params.at(1).back() == 't' && params.empty())
						setTopicRestriction(cfd, channel, params.at(1));
					else if (params.at(1).back() == 'k')
						setKey(cfd, channel, params);
					else if (params.at(1).back() == 'l')
						setClientLimit(cfd, channel, params);
				}
				return ;
			}
		}
		std::string	msg = ":ircserv 482 " + _clients.at(getClientIndex(cfd)).getUser() + " #" + params.at(0) + " :You're not channel operator\r\n";
		send(cfd, msg.c_str(), msg.length(), 0);
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
