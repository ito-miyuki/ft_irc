#include "../Server.hpp"

void	Server::welcomeClient(int cfd, Channel &channel, Client &client)
{
	std::string msg = ":" + client.getNick() + "!~" + client.getUser() + "@" + client.getIPa() + " JOIN " + channel.getChannelName() + "\r\n";
	send(cfd, msg.c_str(), msg.length(), 0);
	if (!channel.getTopic().empty())
	{
		msg = ":ft_irc 332 " + client.getNick() + " " + channel.getChannelName() + " :" + channel.getTopic() + "\r\n";
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


void	Server::addNewChannel(int cfd, std::string channelName, std::string channelKey) {
	
	std::string	key = "";
	Client	&client = _clients.at(getClientIndex(cfd));

	if (!channelKey.empty()) {

		if (isValidKey(channelKey))
			key = channelKey;
		else {

			std::string msg = ":ft_irc 525 " + client.getUser() + " " + channelName + " :Key is not well-formed\r\n";
			send(cfd, msg.c_str(), msg.length(), 0);
			return ;
		}
	}
	Channel newChannel(channelName, cfd, key);
	addChannel(newChannel);
	client.addOpChannel(channelName);
	welcomeClient(cfd, newChannel, client);
}

void	Server::joinChannel(int cfd, std::string channelName, std::string channelKey) {

	int	channelIndex = getChannelIndex(channelName);

	if (channelIndex > -1) {

		Channel	&channel = _channels.at(channelIndex);
		if (!alreadyJoint(cfd, channel.getJointClients())) {

			if (checkKey(cfd, channel, channelKey) && checkInvite(cfd, channel) && checkLimit(cfd, channel)) {

				Client	&client = _clients.at(getClientIndex(cfd));
				client.addChannel(channelName);
				channel.addClient(cfd);
				if (isInvited(cfd, channel.getInvitedClients()))
					channel.removeInvite(cfd);
				welcomeClient(cfd, channel, client);
				std::string msg = ":" + client.getNick() + "!" + client.getUser() + "@" + client.getIPa() + " JOIN " + channel.getChannelName() + "\r\n";
				channel.broadcast(msg, cfd, false);
			}
		}
	} else {

		if (isValidName(channelName))
			addNewChannel(cfd, channelName, channelKey);
		else {

			std::string msg = ":ft_irc 476 " + channelName + " :Bad Channel Mask\r\n";
			send(cfd, msg.c_str(), msg.length(), 0);
		}
	}
}

void	Server::join(int cfd, std::string arg)
{
	std::vector<std::string>	params;

	parser(arg, params, ' ');
	if (params.at(0) == "JOIN") {

		if (params.size() <= 1) {

			std::string msg = ":ft_irc 461 JOIN :Not enough parameters\r\n";
			send(cfd, msg.c_str(), msg.length(), 0);
			return ;
		} else {

			std::vector<std::string> channels;
			std::vector<std::string> keys;

			parser(params.at(1), channels, ',');
			if (params.size() > 2)
				parser(params.at(2), keys, ',');
			for (std::size_t index = 0; index < channels.size(); index++) {

				std::string	key = "";

				if (keys.size() >= index + 1)
					key = keys.at(index);
				joinChannel(cfd, channels.at(index), key);
			}
		}
	} else {

		std::string msg = ":ft_irc 421 " + _clients.at(getClientIndex(cfd)).getUser() + " " + params.at(0) + " :Unknown command\r\n";
		send(cfd, msg.c_str(), msg.length(), 0);
	}
}
