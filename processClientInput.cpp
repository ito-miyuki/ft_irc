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

bool Server::isUserInChannel(const std::string& channelName, int userFd) {

    for (std::vector<Channel>::iterator ite = _channels.begin(); ite != _channels.end(); ++ite) {
        if (ite->getChannelName() == channelName) {
            std::vector<int> jointClients = ite->getJointClients();
            std::vector<int> operators = ite->getOps();

            std::vector<int>::iterator found = std::find(jointClients.begin(), jointClients.end(), userFd);
            std::vector<int>::iterator foundInOps = std::find(operators.begin(), operators.end(), userFd);
            
            if(found != jointClients.end() || foundInOps != operators.end()) {
                return true;
            }
            return false;
        }
    }
    return false;
}

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

void	Server::processClientInput(size_t *clientIndex, int cfd)
{
	char buffer[1024] = {0};
	int byteRead = recv(cfd, buffer, sizeof(buffer), 0);

	if (byteRead <= 0) {
		std::cout << "Client disconnected: " << cfd << std::endl;
		std::string msg = "ERROR :Connection timeout\r\n";
		send(cfd, msg.c_str(), msg.length(), 0);
		eraseClient(cfd, clientIndex);
		return ;
	}
	
	std::stringstream	ss(buffer);
	processInputData(ss, cfd, clientIndex);
}
