#include "../Server.hpp"

void	Server::removeClientFromChannels(int cfd) {

	if (!_channels.empty()) {

		for (std::vector<Channel>::iterator it = _channels.begin(); it != _channels.end(); std::advance(it, 1)) {

			if (isUserInChannel(it->getChannelName(), cfd)) {

				it->removeClient(cfd);
				it->removeOp(cfd);
			}
		}
	}
}

void	Server::eraseClient(int cfd, size_t *clientIndex) {

	int	cIndex = getClientIndex(cfd);

	if (cIndex > -1) {

		std::cout << "Client disconnected: " << cfd << std::endl;
		removeClientFromChannels(cfd);
		_clients.erase(_clients.begin() + cIndex);
		_fds.erase(_fds.begin() + *clientIndex);
		close(cfd);
		removeDeadChannels();
		(*clientIndex)--;
	}
}

void	Server::processInputData(std::stringstream &ss, int cfd, size_t *clientIndex) {

	std::string	arg;

	while (getline(ss, arg)) {

		//std::cout <<  "this is our arg = <" << arg << ">" << std::endl; // for eval
		if (arg.empty())
			continue ;
		if (!arg.empty() && arg.back() == '\r')
			arg.pop_back();
		if (!isRegistered(cfd)) {

			registerClient(cfd, arg, clientIndex);
		} else {

			runCommand(cfd, arg, clientIndex);
		}
	}
}


void	Server::processClientInput(size_t *clientIndex, int cfd) {
	
	char buffer[1024] = {0};
	int byteRead = recv(cfd, buffer, sizeof(buffer), 0);

	if (byteRead <= 0) {
		//std::cout << "Client disconnected: " << cfd << std::endl;
		std::string msg = "ERROR :Connection timeout\r\n";
		send(cfd, msg.c_str(), msg.length(), 0);
		eraseClient(cfd, clientIndex);
		return ;
	}

	buffer[byteRead] = '\0';
	_clientBuffers[cfd] += buffer;

	if (_clientBuffers[cfd].back() != '\n') {
        return;
    }

	size_t pos;
	while ((pos = _clientBuffers[cfd].find("\n")) != std::string::npos) {
		
		std::string command = _clientBuffers[cfd].substr(0, pos);

		if (!command.empty() && command.back() == '\r') {
			command.pop_back();
		}

		_clientBuffers[cfd].erase(0, pos + 1);

		std::stringstream	ss(command);
		processInputData(ss, cfd, clientIndex);
	}
}