#include "Server.hpp"

bool	Server::verifyParams(int cfd, std::string channel, std::string mode)
{
	Channel	chnl = getChannel(channel);

	if (chnl.getChannelName().empty())
	{
		std::string msg = ":ircserv 403 " + getClient(cfd).getUser() + " " + channel + " :No such channel\r\n";
		send(getClient(cfd).getFd(), msg.c_str(), msg.length(), 0);
		return (false);
	}
	if (mode != "+i" && mode != "-i" && mode != "+t" && mode != "-t"
		&& mode != "+k" && mode != "-k" && mode != "+o" && mode != "-o"
		&& mode != "+l" && mode != "-l")
		// do we need to inform the client about the channel modes...?
		return (false);
	return (true);
}

void	Server::setInviteStatus(int cfd, std::string channel, std::string mode)
{
	Channel	chnl = getChannel(channel);

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
	Channel	chnl = getChannel(channel);

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
	Channel	chnl = getChannel(channel);

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
	Channel	chnl = getChannel(channel);

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

void	Server::setMode(int cfd, std::string channel, std::string mode, std::string param)
{
	if (verifyParams(cfd, channel, mode))
	{
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
	std::stringstream	ss(arg);
	std::string			substr;
	char				del = ' ';
	int					argCount = 0;
	std::string			channel = "";
	std::string			mode = "";
	std::string			param = "";

	while (getline(ss, substr, del))
	{
		if (argCount == 1)
			channel = substr;
		if (argCount == 2)
			mode = substr;
		if (argCount == 3)
			param = substr;
		argCount++;
	}
	if (channel.empty())
	{
		std::string msg = ":ircserv 461 " + getClient(cfd).getUser() + " MODE :Not enough parameters\r\n";
		send(getClient(cfd).getFd(), msg.c_str(), msg.length(), 0);
		return ;
	}
	setMode(cfd, channel, mode, param);
}
