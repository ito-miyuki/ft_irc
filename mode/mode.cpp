#include "../Server.hpp"

bool	Server::verifyParams(int cfd, std::vector<std::string> &params)
{
	int	chIndex = getChannelIndex(params.at(1));

	if (chIndex < 0)
	{
		if (!isClient(params.at(1)))
		{
			std::string msg = ":ft_irc 403 " + _clients.at(getClientIndex(cfd)).getUser() + " " + params.at(1) + " :No such channel\r\n";
			send(cfd, msg.c_str(), msg.length(), 0);
		}
		return (false);
	}
	if (params.size() > 2 && params.at(2) != "+i" && params.at(2) != "-i" && params.at(2) != "+t" && params.at(2) != "-t"
		&& params.at(2) != "+k" && params.at(2) != "-k" && params.at(2) != "+o" && params.at(2) != "-o"
		&& params.at(2) != "+l" && params.at(2) != "-l")
		return (false);
	return (true);
}

void	Server::returnChannelMode(int cfd, Channel &channel) {

	std::string msg = ":ft_irc 324 " + _clients.at(getClientIndex(cfd)).getNick() + " " + channel.getChannelName() + " ";
				
	if (channel.getClientLimit() > -1)
		msg = msg + "l";
	if (channel.isInviteOnly())
		msg = msg + "i";
	if (channel.isTopicRestricted())
		msg = msg + "t";
	if (!channel.getKey().empty())
		msg = msg + "k";
	msg = msg + "\r\n";
	send(cfd, msg.c_str(), msg.length(), 0);
}


// MODE #channel flag user/key/value(optional)
void	Server::setMode(int cfd, std::vector<std::string> &params) {

	if (verifyParams(cfd, params)) {

		Channel &channel = _channels.at(getChannelIndex(params.at(1)));

		if (hasOpRights(cfd, params.at(1))) {

			if (params.size() == 2) {

				returnChannelMode(cfd, channel);

			} else if (params.size() == 3) {

				if (params.at(2).back() == 'i')
					setInviteStatus(cfd, channel, params.at(2));
				else if (params.at(2).back() == 't')
					setTopicRestriction(cfd, channel, params.at(2));
				else if (params.at(2) == "-l")
					setClientLimit(cfd, channel, params);
				else {

					std::string msg = ":ft_irc 461 " + _clients.at(getClientIndex(cfd)).getUser() + " MODE :Not enough parameters\r\n";
					send(cfd, msg.c_str(), msg.length(), 0);
				}

			} else if (params.size() == 4) {

				if (params.at(2).back() == 'o')
					setOpRights(cfd, params);
				else if (params.at(2).back() == 'k')
					setKey(cfd, channel, params);
				else if (params.at(2) == "+l")
					setClientLimit(cfd, channel, params);
				else {

					std::string msg = ":ft_irc 461 " + _clients.at(getClientIndex(cfd)).getUser() + " MODE :Incorrect number of parameters\r\n";
					send(cfd, msg.c_str(), msg.length(), 0);
				}
			}

		} else if (params.size() == 2 && channel.containSender(cfd)) {

			returnChannelMode(cfd, channel);

		} else {

			std::string	msg = ":ft_irc 482 " + _clients.at(getClientIndex(cfd)).getUser() + " " + params.at(1) + " :You're not channel operator\r\n";
			send(cfd, msg.c_str(), msg.length(), 0);
		}
	}
}

void	Server::mode(int cfd, std::string arg) {

	std::vector<std::string>	params;

	parser(arg, params, ' ');
	if (params.at(0) == "MODE") {

		if (params.size() <= 1) {

			std::string msg = ":ft_irc 461 " + _clients.at(getClientIndex(cfd)).getUser() + " MODE :Not enough parameters\r\n";
			send(cfd, msg.c_str(), msg.length(), 0);
			return ;
		}
		setMode(cfd, params);

	} else {
		
		std::string msg = ":ft_irc 421 " + _clients.at(getClientIndex(cfd)).getUser() + " " + params.at(0) + " :Unknown command\r\n";
		send(cfd, msg.c_str(), msg.length(), 0);
	}
}
