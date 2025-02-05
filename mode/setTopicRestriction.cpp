#include "../Server.hpp"

void	Server::setTopicRestriction(int cfd, Channel &channel, std::string mode) {

	if (mode.front() == '+' && !channel.isTopicRestricted()) {

		channel.setTopicRestricted(true);
		std::string msg = ":" + _clients.at(getClientIndex(cfd)).getNick() + "!" + _clients.at(getClientIndex(cfd)).getUser() + "@" + _clients.at(getClientIndex(cfd)).getIPa() + " MODE " + channel.getChannelName() + " " + mode + "\r\n";
		channel.broadcast(msg, cfd, true);
	} else if (mode.front() == '-' && channel.isTopicRestricted()) {

		channel.setTopicRestricted(false);
		std::string msg = ":" + _clients.at(getClientIndex(cfd)).getNick() + "!" + _clients.at(getClientIndex(cfd)).getUser() + "@" + _clients.at(getClientIndex(cfd)).getIPa() + " MODE " + channel.getChannelName() + " " + mode + "\r\n";
		channel.broadcast(msg, cfd, true);
	}
}
