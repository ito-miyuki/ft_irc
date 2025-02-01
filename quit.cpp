#include "Server.hpp"

void	Server::notifyChannels(int cfd, std::string msg)
{
	std::vector<Channel*>	&ops = _clients.at(getClientIndex(cfd)).getOpChannels();
	std::vector<Channel*>	&regular = _clients.at(getClientIndex(cfd)).getJointChannels();

	if (!ops.empty())
	{
		for (std::vector<Channel*>::iterator it = ops.begin(); it != ops.end(); std::advance(it, 1))
		{
			(*it)->broadcast(msg, cfd, false);
		}
	}
	if (!regular.empty())
	{
		for (std::vector<Channel*>::iterator it = regular.begin(); it != regular.end(); std::advance(it, 1))
		{
			(*it)->broadcast(msg, cfd, false);
		}
	}
}

void	Server::quit(int cfd, std::string arg, size_t *clientIndex)
{
	std::vector<std::string>	params;

	parser(arg, params);
	std::string msg = ":" + _clients.at(getClientIndex(cfd)).getNick() + "!~" + _clients.at(getClientIndex(cfd)).getUser() + "@" + _clients.at(getClientIndex(cfd)).getIPa() + " QUIT " + params.at(0) + "\r\n";
	//notifyChannels(cfd, msg);
	send(cfd, msg.c_str(), msg.length(), 0);
	clearChannelData(cfd, _clients.at(getClientIndex(cfd)));
	_clients.at(getClientIndex(cfd)).clearAllChannels();
	_fds.erase(_fds.begin() + *clientIndex);
	_clients.erase(_clients.begin() + getClientIndex(cfd));
	close(cfd);
	(*clientIndex)--;
}
