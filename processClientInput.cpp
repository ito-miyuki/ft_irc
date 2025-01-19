#include "Server.hpp"

void	Server::eraseClient(int cfd)
{
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); std::advance(it, 1))
	{
		if (it->getFd() == cfd)
		{
			_clients.erase(it);
			decrementClientAmount();
			return ;
		}
	}
	std::cerr << "Could not erase client, check your code motherfuckers" << std::endl;
}

void	Server::processInputData(std::stringstream &ss, int cfd)
{
	std::string	arg;

	while (getline(ss, arg))
	{
		std::cout <<  "this is our arg = <" << arg << ">" << std::endl;
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
		eraseClient(cfd);
		(*clientIndex)--; // Adjust the loop index due to the removal
		return ;
	}
	
	std::stringstream	ss(buffer);
	processInputData(ss, cfd);
}

	//send(_fds[clientIndex].fd, buffer, byteRead, 0);


					