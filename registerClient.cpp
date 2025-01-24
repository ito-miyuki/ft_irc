#include "Server.hpp"

bool	Server::isRegistered(int cfd)
{
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); std::advance(it, 1))
	{
		if (it->getFd() == cfd)
			return (it->isRegistered());
	}
	std::cerr << "You have done the impossible (8" << std::endl;
	return (false);
}

void	Server::registerPassword(Client& client, std::string arg)
{
	if (arg.compare(0, 5, "PASS ") == 0) {
		std::string pwd = arg.substr(5, std::string::npos);
		if (pwd == getPassword()) {
			std::string msg = "Password accepted.\n";
			send(client.getFd(), msg.c_str(), msg.length(), 0);
			client.setPassword(pwd);
		} else	{
			std::string msg = "ERROR: Incorrect password. Connection closed.\n"; // double checking needed
			send(client.getFd(), msg.c_str(), msg.length(), 0);
			close(client.getFd());
			std::cerr << "Client provided an incorrect password. Connection terminated." << std::endl; // double checking needed
			eraseClient(client.getFd());
		}
	}
}

bool	Server::isUniqueNick(std::string nick)
{
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); std::advance(it, 1))
	{
		if (it->getNick() == nick)
			return false;
	}
	return true;
}

// cannot have the same nickname as anyone else | misses capital letter check!!!
void	Server::registerNickname(Client& client, std::string arg)
{
	if (arg.compare(0, 5, "NICK ") == 0) {
		std::string nick = arg.substr(5, std::string::npos);
		int	nickCount = 1;

		std::string newNick = nick;
		while (!isUniqueNick(newNick))
		{
			newNick = nick + std::to_string(nickCount);
			nickCount++;
		}
		client.setNickname(newNick);
		std::cout << "User's nickname: " << client.getNick() << std::endl;
	}
}

// can have the same username as someone else
void	Server::registerUser(Client& client, std::string arg)
{
	if (arg.compare(0, 5, "USER ") == 0) {
		std::string user = arg.substr(5, std::string::npos);
		size_t divider = user.find(' ');
		if (divider != std::string::npos)
		{
			client.setUsername(user.substr(0, divider));
			std::cout << "User's username: " << client.getUser() << std::endl;
		}
		else
			std::cerr << "Failed to find username" << std::endl;
	}
}

void	Server::authenticate(Client &client, std::string arg)
{
	if (client.getPassword().empty())
		registerPassword(client, arg);
	if (client.getNick().empty())
		registerNickname(client, arg);
	if (client.getUser().empty())
		registerUser(client, arg);
	if (!client.getPassword().empty() && !client.getNick().empty() && !client.getUser().empty())
	{
		std::string msg = ":ircserv 001 " + client.getUser() + " :Welcome to the IRC Network, " + client.getNick() + "!\r\n"; // double checking needed
		send(client.getFd(), msg.c_str(), msg.length(), 0);
		client.setAsRegistered();
	}
}

void	Server::registerClient(int cfd, std::string arg)
{
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); std::advance(it, 1))
	{
		if (it->getFd() == cfd)
		{
			authenticate(*it, arg); // if something sketchy happens, check this call
			return ;
		}
	}
}
