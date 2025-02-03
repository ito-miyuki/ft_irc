#include "Server.hpp"

bool	Server::isInvited(int cfd, std::vector<int> &invitedClients)
{
	std::vector<int>::iterator result = std::find(invitedClients.begin(), invitedClients.end(), cfd);
	if (result != invitedClients.end())
		return (true);
	return (false);
}

bool	Server::alreadyJoint(int cfd, std::vector<int> &jointClients)
{
	std::vector<int>::iterator result = std::find(jointClients.begin(), jointClients.end(), cfd);
	if (result != jointClients.end())
		return (true);
	return (false);
}

bool	Server::checkKey(int cfd, Channel &channel, std::string key)
{
	if (!key.empty())
	{
		if (channel.getKey() != key)
		{
			std::string msg = ":ft_irc 475 " + _clients.at(getClientIndex(cfd)).getNick() + " " + channel.getChannelName() + " :Cannot join channel (+k)\r\n"; //find out if IRC stops processing the whole command if error is found...
			send(cfd, msg.c_str(), msg.length(), 0);
			return (false);
		}
	}
	return (true);
}

bool	Server::checkInvite(int cfd, Channel &channel)
{
	if (channel.isInviteOnly())
	{
		if (!isInvited(cfd, channel.getInvitedClients()))
		{
			std::string msg = ":ft_irc 473 " + _clients.at(getClientIndex(cfd)).getNick() + " " + channel.getChannelName() + " :Cannot join channel (+i)\r\n"; //find out if IRC stops processing the whole command if error is found...
			send(cfd, msg.c_str(), msg.length(), 0);
			return (false);
		}
	}
	return (true);
}

bool	Server::checkLimit(int cfd, Channel &channel)
{
	if (channel.getClientLimit() != -1)
	{
		if (static_cast<std::size_t>(channel.getClientLimit()) <= (channel.getJointClients().size() + channel.getOps().size()))
		{
			std::string msg = ":ft_irc 471 " + _clients.at(getClientIndex(cfd)).getNick() + " " + channel.getChannelName() + " :Cannot join channel (+l)\r\n";
			send(cfd, msg.c_str(), msg.length(), 0);
			return (false);
		}
	}
	return (true);
}

void	Server::welcomeClient(int cfd, Channel &channel, Client &client)
{
	std::string msg = ":" + client.getNick() + "!~" + client.getUser() + "@" + client.getIPa() + " JOIN " + channel.getChannelName() + "\r\n";
	send(cfd, msg.c_str(), msg.length(), 0);
	if (!channel.getTopic().empty())
	{
		msg = ":ft_irc 332 " + client.getNick() + " @ " + channel.getChannelName() + " :" + channel.getTopic() + "\r\n";
		send(cfd, msg.c_str(), msg.length(), 0);
	}
	msg = ":ft_irc 353 " + client.getNick() + " @ " + channel.getChannelName() + " :";
	std::vector<int>	jointClients = channel.getJointClients();
	if (!jointClients.empty())
	{
		for (std::vector<int>::iterator it = jointClients.begin(); it != jointClients.end(); std::advance(it, 1))
		{
			int	cIndex = getClientIndex(*it);
			msg = msg + _clients.at(cIndex).getNick() + " ";
		}
	}
	std::vector<int>	ops = channel.getOps();
	if (!ops.empty())
	{
		for (std::vector<int>::iterator it = ops.begin(); it != ops.end(); std::advance(it, 1))
		{
			int	cIndex = getClientIndex(*it);
			msg = msg + "@" + _clients.at(cIndex).getNick() + " ";
		}
	}
	msg.pop_back();
	msg = msg + "\r\n";
	send(cfd, msg.c_str(), msg.length(), 0);
	msg = ":ft_irc 366 " + client.getNick() + " " + channel.getChannelName() + " :End of /NAMES list\r\n";
	send(cfd, msg.c_str(), msg.length(), 0);
}


void	Server::addNewChannel(int cfd, std::string channelName, std::string channelKey)
{
	std::string	key = "";
	Client	&client = _clients.at(getClientIndex(cfd));

	if (!channelKey.empty())
		key = channelKey;
	Channel newChannel(channelName, cfd, key);
	addChannel(newChannel);
	client.addOpChannel(channelName);
	welcomeClient(cfd, newChannel, client);
}

void	Server::joinChannel(int cfd, std::vector<std::string> &params)
{
	int			channelIndex = getChannelIndex(params.at(0));
	std::string	key = "";

	if (params.size() > 1)
		key = params.at(1);
	if (channelIndex > -1)
	{
		Channel	&channel = _channels.at(channelIndex);
		if (!alreadyJoint(cfd, channel.getJointClients()))
		{
			if (checkKey(cfd, channel, key) && checkInvite(cfd, channel) && checkLimit(cfd, channel))
			{
				Client	&client = _clients.at(getClientIndex(cfd));
				client.addChannel(params.at(0));
				channel.addClient(cfd);
				if (isInvited(cfd, channel.getInvitedClients()))
					channel.removeInvite(cfd);
				welcomeClient(cfd, channel, client);
				std::string msg = ":" + client.getNick() + "!" + client.getUser() + "@" + client.getIPa() + " JOIN " + channel.getChannelName() + "\r\n";
				channel.broadcast(msg, cfd, false);
			}
		}
	}
	else
	{
		addNewChannel(cfd, params.at(0), key);
	}
}

std::string	Server::parseChannelInfo(std::string channels)
{
	if (channels.find(',') != std::string::npos)
	{
		std::stringstream	ss(channels);
		std::string			channel;
		char				del = ',';
		int					commas = std::count(channels.begin(), channels.end(), del);

		while (getline(ss, channel, del))
		{
			if (commas == 0)
				return (channel);
			commas--;
		}
	}
	return (channels);
}

void	Server::join(int cfd, std::string arg)
{
	std::vector<std::string>	params;

	parser(arg, params);
	if (params.empty() || params.at(0).empty())
	{
		std::string msg = ":ft_irc 461 JOIN :Not enough parameters\r\n";
		send(cfd, msg.c_str(), msg.length(), 0);
		return ;
	}
	else
	{
		params.at(0) = parseChannelInfo(params.at(0));
		joinChannel(cfd, params);
	}
}

/* bool	Server::isValidName(std::string channel) // this may be unnecessary operation
{
	std::regex	correct("^[&#][^\\s,^\x07]+$");

	if (std::regex_match(channel, correct))
		return (true);
	return (false);
} */

/* void	Server::verifyChannels(int cfd, std::vector<std::string> &channels, std::vector<std::string> &keys)
{
	int	index = 0;

	for (std::vector<std::string>::iterator begin = channels.begin(); begin != channels.end(); std::advance(begin, 1))
	{
		if (!isValidName(*begin))
		{
			if (!keys.empty())
				keys.erase(keys.begin() + index);
			std::string msg = ":ft_irc 476 " + *begin + " :Bad Channel Mask\r\n"; //find out if IRC stops processing the whole command if error is found...
			send(getClient(cfd).getFd(), msg.c_str(), msg.length(), 0);
			channels.erase(channels.begin() + index);
		}
		index++;
	}
} */