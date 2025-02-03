#include "Server.hpp"

void	Server::notifyChannels(int cfd, std::string msg)
{
	std::vector<std::string>	&ops = _clients.at(getClientIndex(cfd)).getOpChannels();
	std::vector<std::string>	&regular = _clients.at(getClientIndex(cfd)).getJointChannels();

	if (!ops.empty())
	{
		for (std::vector<std::string>::iterator it = ops.begin(); it != ops.end(); std::advance(it, 1))
		{
			_channels.at(getChannelIndex(*it)).broadcast(msg, cfd, false);
		}
	}
	if (!regular.empty())
	{
		for (std::vector<std::string>::iterator it = regular.begin(); it != regular.end(); std::advance(it, 1))
		{
			_channels.at(getChannelIndex(*it)).broadcast(msg, cfd, false);
		}
	}
}

void	Server::removeDeadChannels()
{
	for (std::size_t index = 0; index < _channels.size(); index++)
	{
		if (_channels.at(index).getJointClients().size() + _channels.at(index).getOps().size() == 0)
		{
			_channels.erase(_channels.begin() + index);
		}
	}
}

void	Server::quit(int cfd, std::string arg, size_t *clientIndex)
{
	std::size_t	pos = arg.find(':');
	std::string	reason;
	int			cIndex = getClientIndex(cfd);

	if (pos == std::string::npos)
	{
		std::cerr << "Error: cannot process QUIT request" << std::endl;
		return ;
	}
	reason = arg.substr(pos);
	if (reason.compare(":leaving") == 0)
		reason = ":Client Quit";
	std::string msg = ":" + _clients.at(cIndex).getNick() + "!~" + _clients.at(cIndex).getUser() + "@" + _clients.at(cIndex).getIPa() + " QUIT " + reason + "\r\n";
	send(cfd, msg.c_str(), msg.length(), 0);
	notifyChannels(cfd, msg);
	eraseClient(cfd, clientIndex);
}
