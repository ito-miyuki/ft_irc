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

void	Server::removeClientFromChannels(int cfd)
{
	if (!_channels.empty())
	{
		for (std::vector<Channel>::iterator it = _channels.begin(); it != _channels.end(); std::advance(it, 1))
		{
			if (isUserInChannel(it->getChannelName(), cfd))
			{
				it->removeClient(cfd);
				it->removeOp(cfd);
			}
		}
	}
}

void	Server::eraseClient(int cfd, size_t *clientIndex)
{
	int	cIndex = getClientIndex(cfd);

	if (cIndex > -1)
	{
		removeClientFromChannels(cfd);
		_clients.erase(_clients.begin() + cIndex);
		_fds.erase(_fds.begin() + *clientIndex);
		close(cfd);
		removeDeadChannels();
		(*clientIndex)--;
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
			registerClient(cfd, arg, clientIndex);
		}
		else
		{
			runCommand(cfd, arg, clientIndex);
		}
	}
}

std::map<int, std::string> clientBuffers; // trying global variable

void	Server::processClientInput(size_t *clientIndex, int cfd)
{
	char buffer[1024] = {0};
	int byteRead = recv(cfd, buffer, sizeof(buffer), 0);

	std::cout << "recv() recieved: " << byteRead << std::endl; // for testing 

	if (byteRead <= 0) {
		std::cout << "Client disconnected: " << cfd << std::endl;
		std::string msg = "ERROR :Connection timeout\r\n";
		send(cfd, msg.c_str(), msg.length(), 0);
		eraseClient(cfd, clientIndex);
		return ;
	}

	// // //new implementing //

	// buffer[byteRead] = '\0';
	// clientBuffers[cfd] += buffer;

	// std::cout << "Received buffer: [" << buffer << "]" << std::endl; // Debugging
    // std::cout << "Current buffer: [" << clientBuffers[cfd] << "]" << std::endl; // Debugging

	// if (clientBuffers[cfd].back() != '\n') {
    //     std::cout << "Waiting for more input..." << std::endl; // for debugging
    //     return;
    // }

	// size_t pos;
	// while ((pos = clientBuffers[cfd].find("\r\n")) != std::string::npos) {
	// 	std::string command = clientBuffers[cfd].substr(0, pos);
	// 	clientBuffers[cfd].erase(0, pos + 2);

	// 	std::cout << "Processing command: " << command << std::endl; // for debugging

	// 	std::stringstream	ss(command);
	// 	processInputData(ss, cfd, clientIndex);

	// }
	// ////
	
	std::stringstream	ss(buffer);
	processInputData(ss, cfd, clientIndex);
}