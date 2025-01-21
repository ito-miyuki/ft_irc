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

void	Server::checkKey(int cfd, std::vector<Channel>::iterator &channel, std::vector<std::string> &keys, bool *canJoin, int index)
{
	if (channel->hasKey() && !keys.empty())
	{
		*canJoin = channel->getKey() == keys.at(index);
		if (!canJoin)
		{
			std::string msg = ":ft_irc 475 " + getClient(cfd)->getNick() + " " + channel->getChannelName() + " :Cannot join channel (+k)\r\n"; //find out if IRC stops processing the whole command if error is found...
			send(getClient(cfd)->getFd(), msg.c_str(), msg.length(), 0);
		}
	}
	else
		*canJoin = true;
}

void	Server::checkInvite(int cfd, std::vector<Channel>::iterator &channel, bool *canJoin)
{
	if (channel->isInviteOnly())
	{
		*canJoin = isInvited(cfd, channel->getInvitedClients());
		if (!canJoin)
		{
			std::string msg = ":ft_irc 473 " + getClient(cfd)->getNick() + " " + channel->getChannelName() + " :Cannot join channel (+i)\r\n"; //find out if IRC stops processing the whole command if error is found...
			send(getClient(cfd)->getFd(), msg.c_str(), msg.length(), 0);
		}
	}
	else
		*canJoin = true;
}

void	Server::checkLimit(int cfd, std::vector<Channel>::iterator &channel, bool *canJoin)
{
	if (channel->getClientLimit() != -1)
	{
		*canJoin = channel->getClientLimit() < channel->getJointClients().size();
		if (!canJoin)
		{
			std::string msg = ":ft_irc 471 " + getClient(cfd)->getNick() + " " + channel->getChannelName() + " :Cannot join channel (+l)\r\n"; //find out if IRC stops processing the whole command if error is found...
			send(getClient(cfd)->getFd(), msg.c_str(), msg.length(), 0);
		}
	}
	else
		*canJoin = true;
}

void	Server::welcomeClient(int cfd, std::vector<Channel>::iterator &channel, int channelAmount)
{
	channel->getJointClients().push_back(cfd);
	getClient(cfd)->getJointChannels().push_back(&(*channel));
	if (channelAmount == 1)
	{
		// print appropriate messages
	}
}

void	Server::joinChannels(int cfd, std::vector<std::string> &channels, std::vector<std::string> &keys)
{
	int		index = 0;
	bool	canJoin = false;

	for (std::vector<std::string>::iterator begin = channels.begin(); begin != channels.end(); std::advance(begin, 1))
	{
		std::vector<Channel>::iterator	channel = getChannel(*begin);

		if (channel != _channels.end())
		{
			if (!alreadyJoint(cfd, channel->getJointClients()))
			{
				checkKey(cfd, channel, keys, &canJoin, index);
				checkInvite(cfd, channel, &canJoin);
				checkLimit(cfd, channel, &canJoin);
				if (canJoin)
				{
					welcomeClient(cfd, channel, channels.size());
				}
			}
		}
		else
		{
			// create new channel and add moderator rights
		}
		index++;
	}
}

bool	Server::isValidName(std::string channel)
{
	std::regex	correct("^[&#][^\\s,^\x07]+$");

	if (std::regex_match(channel, correct))
		return (true);
	return (false);
}

void	Server::verifyChannels(int cfd, std::vector<std::string> &channels, std::vector<std::string> &keys)
{
	int	index = 0;

	for (std::vector<std::string>::iterator begin = channels.begin(); begin != channels.end(); std::advance(begin, 1))
	{
		if (!isValidName(*begin))
		{
			if (!keys.empty())
				keys.erase(keys.begin() + index);
			std::string msg = ":ft_irc 476 " + *begin + " :Bad Channel Mask\r\n"; //find out if IRC stops processing the whole command if error is found...
			send(getClient(cfd)->getFd(), msg.c_str(), msg.length(), 0);
			channels.erase(channels.begin() + index);
		}
		index++;
	}
}

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
	verifyChannels(cfd, names, keyList);
	joinChannels(cfd, names, keyList);
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
		std::string msg = ":ft_irc 461 JOIN :Not enough parameters\r\n";
		send(getClient(cfd)->getFd(), msg.c_str(), msg.length(), 0);
		return ;
	}
	else if (channelNames.compare("0"))
	{
		//executeSpecialOperation
	}
	else
	{
		parseChannelInfo(cfd, channelNames, keys);
	}
}
