#include "Server.hpp"

/* bool	Server::isRegistered(size_t clientIndex)
{
	for (size_t i = 0; i < getClientAmount(); i++)
	{
		std::cout << "this is fd" << _clients[0].getFd() << "compared to " << _fds.at(0).fd << std::endl;
		if (_clients[i].getFd() == _fds[clientIndex].fd)
		{
			return (_clients[i].isRegistered());
		}
	}
	std::cerr << "You have done the impossible (8" << std::endl;
	return (false);
} */

bool	Server::isRegistered(int cfd)
{
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); std::advance(it, 1))
	{
		std::cout << "this is fd " << it->getFd() << " compared to " << cfd << std::endl;
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
			// make a correct password message
			client.setPassword(pwd);
		} else	{
			// handle error message and disconnecting
		}
	}
}

// cannot have the same nickname as anyone else
void	Server::registerNickname(Client& client, std::string arg)
{
	if (arg.compare(0, 5, "NICK ") == 0) {
		std::string nick = arg.substr(5, std::string::npos);
		if (!nick.empty()) {
			// make an input validation
			client.setNickname(nick);
		} else	{
			// handle error message and ask again
		}
	}
}

// can have the same username as someone else
void	Server::registerUser(Client& client, std::string arg)
{
	if (arg.compare(0, 5, "USER ") == 0) {
		std::string user = arg.substr(5, std::string::npos);
		if (!user.empty()) {
			// make a correct password message
			client.setUsername(user);
		} else	{
			// handle error message and ask again
		}
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
		// send welcome message
		client.setAsRegistered();
	}
}

/* void	Server::registerClient(size_t clientIndex, std::string arg)
{
	for (size_t i = 0; i < getClientAmount(); i++) 
	{
		if (_clients[i].getFd() == _fds[clientIndex].fd)
		{
			authenticate(_clients[i], arg);
			return ;
		}
	}
} */

void	Server::registerClient(int cfd, std::string arg)
{
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); std::advance(it, 1))
	{
		if (it->getFd() == cfd)
		{
			authenticate(*it, arg);
			return ;
		}
	}
}

/* void	Server::processInputData(std::stringstream &ss, size_t clientIndex)
{
	std::string	arg;

	while (getline(ss, arg))
	{
		std::cout <<  "this is our arg =" << arg << std::endl;
		if (arg.empty())
			continue ;
		if (!arg.empty() && arg.back() == '\r')
			arg.pop_back();
		if (!isRegistered(clientIndex))
		{
			registerClient(clientIndex, arg);
		}
	}
} */

void	Server::processInputData(std::stringstream &ss, int cfd)
{
	std::string	arg;

	while (getline(ss, arg))
	{
		std::cout <<  "this is our arg =" << arg << std::endl;
		if (arg.empty())
			continue ;
		if (!arg.empty() && arg.back() == '\r')
			arg.pop_back();
		if (!isRegistered(cfd))
		{
			registerClient(cfd, arg);
		}
	}
}

void	Server::processClientInput(size_t *clientIndex, int cfd)
{
	char buffer[1024] = {0};
	int byteRead = recv(cfd, buffer, sizeof(buffer), 0);

	if (byteRead <= 0) {
		std::cout << "Client disconnected: " << cfd << std::endl;
		close(cfd);
		_fds.erase(_fds.begin() + *clientIndex);
		(*clientIndex)--; // Adjust the loop index due to the removal
		decrementClientAmount();
		return ;
	}
	
	std::stringstream	ss(buffer);
	processInputData(ss, cfd);
}
/* void	Server::processClientInput(size_t *clientIndex)
{
	char buffer[1024] = {0};
	int byteRead = recv(_fds[*clientIndex].fd, buffer, sizeof(buffer), 0);

	if (byteRead <= 0) {
		std::cout << "Client disconnected: " << _fds[*clientIndex].fd << std::endl;
		close(_fds[*clientIndex].fd);
		_fds.erase(_fds.begin() + *clientIndex);
		(*clientIndex)--; // Adjust the loop index due to the removal
		decrementClientAmount();
		return ;
	}
	
	std::stringstream	ss(buffer);
	processInputData(ss, *clientIndex);
} */

	//send(_fds[clientIndex].fd, buffer, byteRead, 0);


					