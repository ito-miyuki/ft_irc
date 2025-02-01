#include "Server.hpp"

/* void	Server::part(int cfd, Client &client, Channel &channel, std::string reason)
{
	std::string msg;

	if (reason.empty())
		msg = ":" + client.getNick() + "!~" + client.getUser() + "@" + client.getIPa() + " PART " + channel.getChannelName() + "\r\n";
	else
		msg = ":" + client.getNick() + "!~" + client.getUser() + "@" + client.getIPa() + " PART " + channel.getChannelName() + " " + reason + "\r\n";
	send(cfd, msg.c_str(), msg.length(), 0);
	channel.broadcast(msg, cfd, false);
} */

void	Server::clearChannelData(int cfd, Client &client)
{
	std::vector<Channel*>	&ops = client.getOpChannels();
	std::vector<Channel*>	&regular = client.getJointChannels();
	
	if (!ops.empty())
	{
		for (std::vector<Channel*>::iterator it = ops.begin(); it != ops.end(); std::advance(it, 1))
		{
			(*it)->removeOp(cfd);
		}
	}
	if (!regular.empty())
	{
		for (std::vector<Channel*>::iterator it = regular.begin(); it != regular.end(); std::advance(it, 1))
		{
			(*it)->removeClient(cfd);
		}
	}
}

void	Server::eraseClient(int cfd)
{
	int	cIndex = getClientIndex(cfd);

	if (cIndex > -1)
	{
		Client	&client = _clients.at(cIndex);
		clearChannelData(cfd, client);
		std::string msg = "ERROR :Connection timeout\r\n";
		send(cfd, msg.c_str(), msg.length(), 0);
		client.clearAllChannels();
		_clients.erase(_clients.begin() + cIndex);
	}
}

void	Server::processInputData(std::stringstream &ss, int cfd, size_t *clientIndex)
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
		else
		{
			runCommand(cfd, arg, clientIndex);
		}
	}
}

void	Server::processClientInput(size_t *clientIndex, int cfd)
{
	char buffer[1024] = {0};
	int byteRead = recv(cfd, buffer, sizeof(buffer), 0);

	if (byteRead <= 0) {
		std::cout << "Client disconnected: " << cfd << std::endl;
		_fds.erase(_fds.begin() + *clientIndex);
		eraseClient(cfd);
		close(cfd);
		(*clientIndex)--; // Adjust the loop index due to the removal
		return ;
	}
	
	std::stringstream	ss(buffer);
	processInputData(ss, cfd, clientIndex);
}

	//send(_fds[clientIndex].fd, buffer, byteRead, 0);


					