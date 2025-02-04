#include "../Server.hpp"

void	Server::setClientLimit(int cfd, Channel &channel, std::vector<std::string> &params) {

	if (params.at(2).front() == '-' && params.size() < 4) {

		channel.setClientLimit(-1);
		std::string msg = ":" + _clients.at(getClientIndex(cfd)).getNick() + "!" + _clients.at(getClientIndex(cfd)).getUser() + "@" + _clients.at(getClientIndex(cfd)).getIPa() + " MODE " + channel.getChannelName() + " " + params.at(2) + "\r\n";
		channel.broadcast(msg, cfd, true);
	} else if (params.at(2).front() == '+') {

		if (params.size() < 4) {

			std::string msg = ":ft_irc 461 " + _clients.at(getClientIndex(cfd)).getUser() + " MODE :Not enough parameters\r\n";
			send(cfd, msg.c_str(), msg.length(), 0);
			return ;
		}
		int	limit;
		try {

			limit = std::stoi(params.at(3));

		} catch (const std::exception& e) {

			return ;
		}
		if (limit < 0) {

			return ;
		}
		channel.setClientLimit(limit);
		std::string msg = ":" + _clients.at(getClientIndex(cfd)).getNick() + "!" + _clients.at(getClientIndex(cfd)).getUser() + "@" + _clients.at(getClientIndex(cfd)).getIPa() + " MODE " + channel.getChannelName() + " " + params.at(2) + " " + params.at(3) + "\r\n";
		channel.broadcast(msg, cfd, true);
	}
	
}
