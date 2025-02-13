#include "../Server.hpp"

bool	Server::isRegistered(int cfd) {

	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); std::advance(it, 1)) {

		if (it->getFd() == cfd)
			return (it->isRegistered());
	}
	return (false);
}

void	Server::registerPassword(Client& client, std::string arg, size_t *clientIndex) {

	std::size_t	found = arg.find("PASS");

	if (found != std::string::npos && arg.length() > 5) {

		std::string pwd = arg.substr(found + 5);
		if (pwd == getPassword()) {
			std::string msg = "Password accepted.\n";
			send(client.getFd(), msg.c_str(), msg.length(), 0);
			client.setPassword(pwd);
		} else	{
			std::string msg = "ERROR: Incorrect password. Connection closed.\n";
			send(client.getFd(), msg.c_str(), msg.length(), 0);
			std::cerr << "Client provided an incorrect password. Connection terminated." << std::endl;
			eraseClient(client.getFd(), clientIndex);
		}
	}
}

bool	Server::isUniqueNick(std::string nick) {

	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); std::advance(it, 1)) {

		if (it->getNick() == nick)
			return false;
	}
	return true;
}

void	Server::registerNickname(Client& client, std::string arg) {

	std::size_t	found = arg.find("NICK");

	if (found != std::string::npos && arg.length() > 5) {

		if (client.getPassword().empty()) {
			
			std::string msg = "ERROR: Please validate password first.\n";
			send(client.getFd(), msg.c_str(), msg.length(), 0);
			return ;
		}

		std::string nick = arg.substr(found + 5);
		int			nickCount = 1;
		std::string newNick = nick;
		std::regex	incorrect("^([#$:#&+%~,]\\S*|\\S*[,*.?!@ ]\\S*|\\+(q|a|o|h|v)\\S*)");

		if (std::regex_match(newNick, incorrect)) {
			return ;
		}
		while (!isUniqueNick(newNick)) {

			newNick = nick + std::to_string(nickCount);
			nickCount++;
		}
		client.setNickname(newNick);
	}
}

void	Server::registerUser(Client& client, std::string arg) {

	std::size_t	found = arg.find("USER");

	if (found != std::string::npos && arg.length() > 5) {

		if (client.getPassword().empty()) {

			std::string msg = "ERROR: Please validate password first.\n";
			send(client.getFd(), msg.c_str(), msg.length(), 0);
			return ;
		}

		std::string 				user = arg.substr(found);
		std::vector<std::string>	params;

		parser(user, params, ' ');
		if (params.size() >= 4) {

			std::regex	incorrect("^([#$:#&+%~,]\\S*|\\S*[,*.?!@ ]\\S*|\\+(q|a|o|h|v)\\S*)");

			if (std::regex_match(params.at(1), incorrect))
			{
				return ;
			}
			client.setUsername(params.at(1));
		}
	}
}

void	Server::authenticate(Client &client, std::string arg, size_t *clientIndex) {

	if (client.getPassword().empty())
		registerPassword(client, arg, clientIndex);
	if (client.getNick().empty())
		registerNickname(client, arg);
	if (client.getUser().empty())
		registerUser(client, arg);
	if (!client.getPassword().empty() && !client.getNick().empty() && !client.getUser().empty()) {

		std::string msg = ":ft_irc 001 " + client.getNick() + " :Welcome to the IRC Network, " + client.getNick() + "!\r\n";
		send(client.getFd(), msg.c_str(), msg.length(), 0);
		msg = ":ft_irc 002 " + client.getNick() + " :Your host is ft_irc, running version 42\r\n";
		send(client.getFd(), msg.c_str(), msg.length(), 0);
		msg = ":ft_irc 005 " + client.getNick() + " INVITE/MODE/JOIN/KICK/TOPIC/MSG/NICK/QUIT :are supported by this server\r\n";
		send(client.getFd(), msg.c_str(), msg.length(), 0);
		client.setAsRegistered();
	}
}

void	Server::registerClient(int cfd, std::string arg, size_t *clientIndex) {

	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); std::advance(it, 1)) {

		if (it->getFd() == cfd) {

			return (authenticate(*it, arg, clientIndex));
		}
	}
}
