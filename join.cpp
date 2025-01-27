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
	if (channel.hasKey() && !key.empty())
	{
		*canJoin = channel.getKey() == key;
		if (!canJoin)
		{
			std::string msg = ":ircserv 475 " + getClient(cfd).getNick() + " " + channel.getChannelName() + " :Cannot join channel (+k)\r\n"; //find out if IRC stops processing the whole command if error is found...
			send(getClient(cfd).getFd(), msg.c_str(), msg.length(), 0);
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
			std::string msg = ":ircserv 473 " + getClient(cfd).getNick() + " " + channel.getChannelName() + " :Cannot join channel (+i)\r\n"; //find out if IRC stops processing the whole command if error is found...
			send(getClient(cfd).getFd(), msg.c_str(), msg.length(), 0);
		}
	}
	else
		*canJoin = true;
}

void	Server::checkLimit(int cfd, Channel &channel, bool *canJoin)
{
	if (channel.getClientLimit() != -1)
	{
		*canJoin = static_cast<std::size_t>(channel.getClientLimit()) < channel.getJointClients().size();
		if (!canJoin)
		{
			std::string msg = ":ircserv 471 " + getClient(cfd).getNick() + " " + channel.getChannelName() + " :Cannot join channel (+l)\r\n"; //find out if IRC stops processing the whole command if error is found...
			send(getClient(cfd).getFd(), msg.c_str(), msg.length(), 0);
		}
	}
	else
		*canJoin = true;
}

void	Server::welcomeClient(int cfd, Channel &channel)
{
	channel.getJointClients().push_back(cfd);
	getClient(cfd).getJointChannels().push_back(&channel);
	std::string msg = ":" + getClient(cfd).getNick() + "!" + getClient(cfd).getUser() + "@" + getClient(cfd).getIPa() + " JOIN #" + channel.getChannelName() + "\r\n";
	send(getClient(cfd).getFd(), msg.c_str(), msg.length(), 0);
	// print joined clients
	// prints channel stats
}

void	Server::addNewChannel(int cfd, std::string channelName, std::string channelKey)
{
	std::string	key = "";

	if (!channelKey.empty())
		key = channelKey;
	Channel newChannel(channelName, getClient(cfd), key);
	addChannel(newChannel);
	getClient(cfd).addChannel(&newChannel);
	getClient(cfd).addOpChannel(&newChannel);
	welcomeClient(cfd, newChannel);
}

void	Server::joinChannel(int cfd, std::vector<std::string> &channels, std::vector<std::string> &keys)
{
	bool	canJoin = false;
	Channel	&channel = getChannel(channels.at(0));

	if (!channel.getChannelName().empty())
	{
		if (!alreadyJoint(cfd, channel.getJointClients()))
		{
			checkKey(cfd, channel, keys.at(0), &canJoin);
			checkInvite(cfd, channel, &canJoin);
			checkLimit(cfd, channel, &canJoin);
			if (canJoin)
			{
				welcomeClient(cfd, channel);
			}
		}
	}
	else
	{
		addNewChannel(cfd, channels.at(0), keys.at(0));
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

void	Server::parseChannelInfo(int cfd, std::string channelNames, std::string keys)
{
	std::vector<std::string>	names;
	std::vector<std::string>	keyList;
	std::stringstream			ns(channelNames);
	std::string					substr;
	char						del = ',';

	while (getline(ns, substr, del))
		names.push_back(substr);
	if (!keys.empty())
	{
		std::stringstream ks(keys);
		while (getline(ks, substr, del))
			keyList.push_back(substr);
	}
	//verifyChannels(cfd, names, keyList);
	joinChannel(cfd, names, keyList);
}

void	Server::join(int cfd, std::string arg)
{
	std::stringstream	ss(arg);
	std::string			substr;
	char				del = ' ';
	int					argCount = 0;
	std::string			channelNames = "";
	std::string			keys = "";

	while (getline(ss, substr, del))
	{
		if (argCount == 1)
			channelNames = substr;
		if (argCount == 2)
			keys = substr;
		argCount++;
	}
	if (channelNames.empty())
	{
		std::string msg = ":ircserv 461 JOIN :Not enough parameters\r\n";
		send(getClient(cfd).getFd(), msg.c_str(), msg.length(), 0);
		return ;
	}
	else if (channelNames.compare("0") && keys.empty())
	{
		//executeSpecialOperation
	}
	else
	{
		parseChannelInfo(cfd, channelNames, keys);
	}
}
