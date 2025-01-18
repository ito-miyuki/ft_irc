#include "Server.hpp"

bool	Server::isRegistered(size_t clientIndex)
{
	for (size_t i = 0; i < _clientAmount; i++)
	{
		if (_clients[i].getFd() == _fds[clientIndex].fd)
		{
			return (_clients[i].isRegistered());
		}
	}
	std::cerr << "You have done the impossible (8" << std::endl;
	return (false);
}

void	Server::registerPassword(Client& client, std::string arg)
{
	(void)arg;
	(void)client;
}
void	Server::authenticate(Client &client, std::string arg)
{
	if (client.getPassword().empty())
		registerPassword(client, arg);
	if (client.getNick().empty())
		//registerNickname(client, arg);
	if (client.getUser().empty())
		//registerUser(client, arg);
	if (!client.getPassword().empty() && !client.getNick().empty() && !client.getUser().empty())
	{
		// send welcome message
		client.setAsRegistered();
	}
}

void	Server::registerClient(size_t clientIndex, std::string arg)
{
	for (size_t i = 0; i < _clientAmount; i++)
	{
		if (_clients[i].getFd() == _fds[clientIndex].fd)
		{
			authenticate(_clients[i], arg);
			return ;
		}
	}
}

void	Server::processInputData(std::stringstream &ss, size_t clientIndex)
{
	std::string	arg;

	(void)clientIndex;
	while (getline(ss, arg))
	{
		std::cout <<  "this is our arg =" << arg << std::endl;
		if (arg.empty())
			continue ;
		if (!arg.empty() && arg.back() == '\r')
			arg.pop_back();
		/* if (!isRegistered(clientIndex))
		{
			registerClient(clientIndex, arg);
		} */
	}
}


void	Server::processClientInput(size_t *clientIndex)
{
	char buffer[1024] = {0};
	int byteRead = recv(_fds[*clientIndex].fd, buffer, sizeof(buffer), 0);

	if (byteRead <= 0) {
		std::cout << "Client disconnected: " << _fds[*clientIndex].fd << std::endl;
		close(_fds[*clientIndex].fd);
		_fds.erase(_fds.begin() + *clientIndex);
		(*clientIndex)--; // Adjust the loop index due to the removal
		return ;
	}
	
	std::stringstream	ss(buffer);
	processInputData(ss, *clientIndex);
}

	//send(_fds[clientIndex].fd, buffer, byteRead, 0);


					