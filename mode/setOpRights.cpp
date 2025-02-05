#include "../Server.hpp"

void	Server::setOpRights(int cfd, std::vector<std::string> &params) {

	int	targetFd = getUserFdByNick(params.at(3));
	Channel	&opChannel = _channels.at(getChannelIndex(params.at(1)));

	if (opChannel.containSender(targetFd)) {

		Client	&target = _clients.at(getClientIndex(targetFd));

		if (params.at(2).front() == '-') {

			opChannel.removeOp(targetFd);
			opChannel.addClient(targetFd);
			target.removeOpChannel(params.at(1));
			target.addChannel(params.at(1));

			std::string msg = ":" + _clients.at(getClientIndex(cfd)).getNick() + "!" + _clients.at(getClientIndex(cfd)).getUser() + "@" + _clients.at(getClientIndex(cfd)).getIPa() + " MODE " + opChannel.getChannelName() + " " + target.getNick() + " " + params.at(2) + "\r\n";
			opChannel.broadcast(msg, cfd, true);

		} else if (params.at(2).front() == '+') {

			opChannel.removeClient(targetFd);
			opChannel.addOp(targetFd);
			target.removeChannel(params.at(1));
			target.addOpChannel(params.at(1));

			std::string msg = ":" + _clients.at(getClientIndex(cfd)).getNick() + "!" + _clients.at(getClientIndex(cfd)).getUser() + "@" + _clients.at(getClientIndex(cfd)).getIPa() + " MODE " + opChannel.getChannelName() + " " + target.getNick() + " " + params.at(2) + "\r\n";
			opChannel.broadcast(msg, cfd, true);
		}
		return ;
	}
	std::string	msg = ":ft_irc 441 " + _clients.at(getClientIndex(cfd)).getUser() + " " + params.at(1) + " " + params.at(3) + " :They aren't on that channel\r\n";
	send(cfd, msg.c_str(), msg.length(), 0);
}
