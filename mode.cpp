# include "Server.hpp"

bool	Server::verifyParams(int cfd, std::vector<std::string> &params)
{
	Channel	chnl;
	bool	channelExists = getChannel(params.at(0), &chnl);

	if (!channelExists)
	{
		Client	client;
		bool	clientExists = getClient(cfd, &client);

		if (!clientExists)
		{
			std::string msg = ":ircserv 403 " + getClient(cfd).getUser() + " " + params.at(0) + " :No such channel\r\n";
			send(getClient(cfd).getFd(), msg.c_str(), msg.length(), 0);
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

void	Server::setInviteStatus(int cfd, std::string channel, std::string mode)
{
	Channel	chnl;

	(void)cfd;
	getChannel(channel, &chnl);
	if (mode.front() == '+' && !chnl.isInviteOnly())
	{
		chnl.setInviteOnly(true);
		//send info message?
	}
	else if (mode.front() == '-' && chnl.isInviteOnly())
	{
		chnl.setInviteOnly(false);
		//send info message?
	}
}

void	Server::setTopicRestriction(int cfd, std::string channel, std::string mode)
{
	Channel	chnl;

	(void)cfd;
	getChannel(channel, &chnl);
	if (mode.front() == '+' && !chnl.isTopicRestricted())
	{
		chnl.setTopicRestricted(true);
		//send info message?
	}
	else if (mode.front() == '-' && chnl.isTopicRestricted())
	{
		chnl.setTopicRestricted(false);
		//send info message?
	}
}

void	Server::setKey(int cfd, std::string channel, std::string mode, std::string param)
{
	Channel	chnl;

	(void)cfd;
	getChannel(channel, &chnl);
	if (mode.front() == '+')
	{
		if (param.empty())
			//print error
		chnl.setKey(param);
		//send info message?
	}
	else if (mode.front() == '-' && param.empty())
	{
		chnl.setKey("");
		//send info message?
	}
}

void	Server::setClientLimit(int cfd, std::string channel, std::string mode, std::string param)
{
	Channel	chnl;

	(void)cfd;
	getChannel(channel, &chnl);
	if (mode.front() == '+')
	{
		int	limit;
		try
		{
			limit = std::stoi(param);
		}
		catch (const std::exception& e)
		{
			// print error
			return ;
		}
		if (limit < 0)
			//print error
		chnl.setClientLimit(limit);
		//send info message?
	}
	else if (mode.front() == '-' && param.empty())
	{
		chnl.setClientLimit(-1);
		//send info message?
	}
}

bool	Server::hasOpRights(int cfd, std::string channelName){

	Channel	channel;
	bool	channelExists = getChannel(channelName, &channel);

	if (channelExists){

		std::vector<int>	ops = channel.getOps();
		std::vector<int>::iterator result = std::find(ops.begin(), ops.end(), cfd);
		if (result != ops.end())
			return (true);
	}
	return (false);
}

void	Server::setMode(int cfd, std::vector<std::string> &params)
{
	if (verifyParams(cfd, params))
	{
		if (hasOpRights(cfd, params.at(0)))
		//check operator priviledges and print appropriate error
		if (!mode.empty())
		{
			if (mode.back() == 'o')
			{
				// add moderator priviledges
			}
			else if (mode.back() == 'i' && param.empty())
				setInviteStatus(cfd, channel, mode);
			else if (mode.back() == 't' && param.empty())
				setTopicRestriction(cfd, channel, mode);
			else if (mode.back() == 'k')
				setKey(cfd, channel, mode, param);
			else if (mode.back() == 'l')
				setClientLimit(cfd, channel, mode, param);
		}
	}
}

void	Server::mode(int cfd, std::string arg)
{
	std::vector<std::string>	params;

	parser(arg, params);
	if (params.empty())
	{
		std::string msg = ":ircserv 461 " + getClient(cfd).getUser() + " MODE :Not enough parameters\r\n";
		send(getClient(cfd).getFd(), msg.c_str(), msg.length(), 0);
		return ;
	}
	setMode(cfd, params);
}
