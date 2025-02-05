#include "../Server.hpp"

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

bool	Server::checkKey(int cfd, Channel &channel, std::string key) {

	if (key == channel.getKey()) {
		
		return (true);
	}
	std::string msg = ":ft_irc 475 " + _clients.at(getClientIndex(cfd)).getNick() + " " + channel.getChannelName() + " :Cannot join channel (+k)\r\n";
	send(cfd, msg.c_str(), msg.length(), 0);
	return (false);
}

bool	Server::checkInvite(int cfd, Channel &channel)
{
	if (channel.isInviteOnly())
	{
		if (!isInvited(cfd, channel.getInvitedClients()))
		{
			std::string msg = ":ft_irc 473 " + _clients.at(getClientIndex(cfd)).getNick() + " " + channel.getChannelName() + " :Cannot join channel (+i)\r\n";
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

bool	Server::isValidName(std::string channel) {

	std::regex	correct("^[#][^\\s,^\x07]+$");

	if (std::regex_match(channel, correct))
		return (true);
	return (false);
}

bool	Server::isValidKey(std::string channelKey) {
	
	std::regex	correct("^[a-zA-Z0-9]+$");

	if (std::regex_match(channelKey, correct))
		return (true);
	return (false);
}
