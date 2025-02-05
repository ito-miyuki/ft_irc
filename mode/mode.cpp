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
			return (false);
		}
		return (true);
	}
	if (params.size() > 2 && params.at(2) != "+i" && params.at(2) != "-i" && params.at(2) != "+t" && params.at(2) != "-t"
		&& params.at(2) != "+k" && params.at(2) != "-k" && params.at(2) != "+o" && params.at(2) != "-o"
		&& params.at(2) != "+l" && params.at(2) != "-l")
		return (false);
	return (true);
}

void	Server::setMode(int cfd, std::vector<std::string> &params) {

	if (verifyParams(cfd, params)) {

		if (hasOpRights(cfd, params.at(1))) {

			if (params.size() > 2 && !params.at(2).empty()) {

				int	chIndex = getChannelIndex(params.at(1));
				if (params.at(2).back() == 'o' && chIndex < 0)
					setOpRights(cfd, params);
				else {

					Channel &channel = _channels.at(chIndex);

					if (params.at(2).back() == 'i' && params.size() < 4)
						setInviteStatus(cfd, channel, params.at(2));
					else if (params.at(2).back() == 't' && params.size() < 4)
						setTopicRestriction(cfd, channel, params.at(2));
					else if (params.at(2).back() == 'k')
						setKey(cfd, channel, params);
					else if (params.at(2).back() == 'l')
						setClientLimit(cfd, channel, params);
				}
			}

		} else {

			if (params.at(1) != _clients.at(getClientIndex(cfd)).getNick() && params.size() > 2) {

				std::string	msg = ":ft_irc 482 " + _clients.at(getClientIndex(cfd)).getUser() + " " + findCommonChannel(cfd, params.at(1)) + " :You're not channel operator\r\n";
				send(cfd, msg.c_str(), msg.length(), 0);
			}
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

//findCommonChannel(cfd, params.at(1))
