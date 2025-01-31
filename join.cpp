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

void	Server::checkKey(int cfd, Channel &channel, std::string key, bool *canJoin)
{
	if (!key.empty())
	{
		*canJoin = channel.getKey() == key;
		if (!canJoin)
		{
			std::string msg = ":ircserv 475 " + _clients.at(getClientIndex(cfd)).getNick() + " " + channel.getChannelName() + " :Cannot join channel (+k)\r\n"; //find out if IRC stops processing the whole command if error is found...
			send(cfd, msg.c_str(), msg.length(), 0);
		}
	}
	else
		*canJoin = true;
}

void	Server::checkInvite(int cfd, Channel &channel, bool *canJoin)
{
	if (channel.isInviteOnly())
	{
		*canJoin = isInvited(cfd, channel.getInvitedClients());
		if (!canJoin)
		{
			std::string msg = ":ircserv 473 " + _clients.at(getClientIndex(cfd)).getNick() + " " + channel.getChannelName() + " :Cannot join channel (+i)\r\n"; //find out if IRC stops processing the whole command if error is found...
			send(cfd, msg.c_str(), msg.length(), 0);
		}
	}
	else
		*canJoin = true;
}

void	Server::checkLimit(int cfd, Channel &channel, bool *canJoin)
{
	if (channel.getClientLimit() != -1)
	{
		*canJoin = static_cast<std::size_t>(channel.getClientLimit()) < (channel.getJointClients().size() + channel.getOps().size());
		if (!canJoin)
		{
			std::string msg = ":ircserv 471 " + _clients.at(getClientIndex(cfd)).getNick() + " " + channel.getChannelName() + " :Cannot join channel (+l)\r\n";
			send(cfd, msg.c_str(), msg.length(), 0);
		}
	}
	else
		*canJoin = true;
}

void	Server::welcomeClient(int cfd, Channel &channel, Client &client)
{
	std::string msg = ":" + client.getNick() + "!~" + client.getUser() + "@" + client.getIPa() + " JOIN " + channel.getChannelName() + "\r\n";
	send(cfd, msg.c_str(), msg.length(), 0);
	if (!channel.getTopic().empty())
	{
		msg = ": 332 " + client.getNick() + " @ " + channel.getChannelName() + " :" + channel.getTopic() + "\r\n";
		send(cfd, msg.c_str(), msg.length(), 0);
	}
	msg = ": 353 " + client.getNick() + " @ " + channel.getChannelName() + " :";
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
	msg = ": 366 " + client.getNick() + " " + channel.getChannelName() + " :End of /NAMES list\r\n";
	send(cfd, msg.c_str(), msg.length(), 0);
}

void	Server::welcomeClientv2(int cfd, Channel &channel, Client &client)
{
	std::string msg = ":" + client.getNick() + "!~" + client.getUser() + "@" + client.getIPa() + " JOIN " + channel.getChannelName() + "\r\n";
	send(cfd, msg.c_str(), msg.length(), 0);
	if (!channel.getTopic().empty())
	{
		msg = ": 332 " + client.getNick() + " @ " + channel.getChannelName() + " :" + channel.getTopic() + "\r\n";
		send(cfd, msg.c_str(), msg.length(), 0);
	}
	msg = ": 353 " + client.getNick() + " = " + channel.getChannelName() + " :";
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
	msg = ": 366 " + client.getNick() + " " + channel.getChannelName() + " :End of /NAMES list\r\n";
	send(cfd, msg.c_str(), msg.length(), 0);
}

void	Server::addNewChannel(int cfd, std::string channelName, std::string channelKey)
{
	std::string	key = "";
	Client	client = _clients.at(getClientIndex(cfd));

	if (!channelKey.empty())
		key = channelKey;
	Channel newChannel(channelName, cfd, key);
	addChannel(newChannel);
	client.addOpChannel(&newChannel);
	welcomeClient(cfd, newChannel, client);
}

void	Server::joinChannel(int cfd, std::vector<std::string> &params)
{
	bool		canJoin = false;
	int			channelIndex = getChannelIndex(params.at(0));
	std::string	key = "";

	if (params.size() > 1)
		key = params.at(1);
	if (channelIndex > -1)
	{
		Channel	channel = _channels.at(channelIndex);
		if (!alreadyJoint(cfd, channel.getJointClients()))
		{
			checkKey(cfd, channel, key, &canJoin);
			checkInvite(cfd, channel, &canJoin);
			checkLimit(cfd, channel, &canJoin);
			if (canJoin)
			{
				Client	client = _clients.at(getClientIndex(cfd));
				client.addChannel(&channel);
				channel.addClient(cfd);
				if (isInvited(cfd, channel.getInvitedClients()))
					channel.removeInvite(cfd);
				welcomeClientv2(cfd, channel, client);
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

void	Server::leaveAllChannels(int cfd)
{
	Client	client = _clients.at(getClientIndex(cfd));
	std::string msg = ":" + client.getNick() + "!" + client.getUser() + "@" + client.getIPa() + " PART ";

	if (!client.getOpChannels().empty())
	{
		for (std::vector<Channel*>::iterator it = client.getOpChannels().begin(); it != client.getOpChannels().end(); std::advance(it, 1))
		{
			msg = msg + "#" + (*it)->getChannelName() + ",";
		}
		client.getOpChannels().clear();
	}
	for (std::vector<Channel*>::iterator it = client.getJointChannels().begin(); it != client.getJointChannels().end(); std::advance(it, 1))
	{
		msg = msg + "#" + (*it)->getChannelName() + ",";
	}
	msg.pop_back();
	msg = msg + "\r\n";
	send(cfd, msg.c_str(), msg.length(), 0);
	client.getJointChannels().clear();
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
		std::string msg = ":ircserv 461 JOIN :Not enough parameters\r\n";
		send(cfd, msg.c_str(), msg.length(), 0);
		return ;
	}
	else if (params.at(0).compare("0") == 0 && params.size() == 1)
	{
		leaveAllChannels(cfd);
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
			std::string msg = ":ircserv 476 " + *begin + " :Bad Channel Mask\r\n"; //find out if IRC stops processing the whole command if error is found...
			send(getClient(cfd).getFd(), msg.c_str(), msg.length(), 0);
			channels.erase(channels.begin() + index);
		}
		index++;
	}
} */