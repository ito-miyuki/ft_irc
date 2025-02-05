#include "../Server.hpp"

void	Server::setOpRights(int cfd, std::vector<std::string> &params) {

	int	targetFd = getUserFdByNick(params.at(1));
	Client	&target = _clients.at(getClientIndex(targetFd));

	std::string channelName = findChannel(_clients.at(getClientIndex(cfd)), target);

	if (!channelName.empty()) {

		Channel	&opChannel = _channels.at(getChannelIndex(channelName));
		
		if (params.at(2).front() == '-') {

			opChannel.removeOp(target.getFd());
			opChannel.addClient(target.getFd());
			target.removeOpChannel(opChannel.getChannelName());
			target.addChannel(opChannel.getChannelName());

			std::string msg = ":" + _clients.at(getClientIndex(cfd)).getNick() + "!" + _clients.at(getClientIndex(cfd)).getUser() + "@" + _clients.at(getClientIndex(cfd)).getIPa() + " MODE " + target.getNick() + " " + params.at(2) + "\r\n";
			opChannel.broadcast(msg, cfd, true);
		} else if (params.at(2).front() == '+') {

			opChannel.removeClient(target.getFd());
			opChannel.addOp(target.getFd());
			target.removeChannel(opChannel.getChannelName());
			target.addOpChannel(opChannel.getChannelName());

			std::string msg = ":" + _clients.at(getClientIndex(cfd)).getNick() + "!" + _clients.at(getClientIndex(cfd)).getUser() + "@" + _clients.at(getClientIndex(cfd)).getIPa() + " MODE " + target.getNick() + " " + params.at(2) + "\r\n";
			opChannel.broadcast(msg, cfd, true);
		}
		return ;
	}
	std::string	msg = ":ft_irc 441 " + _clients.at(getClientIndex(cfd)).getUser() + " " + params.at(1) + " :They aren't on that channel\r\n";
	send(cfd, msg.c_str(), msg.length(), 0);
}
