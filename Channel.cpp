#include "Channel.hpp"

Channel::Channel(std::string name, Client &op, std::string key) : _name(name), _topic(""), _op(&op), _clientLimit(-1), _inviteOnly(false), _hasKey(false), _topicRestricted(false), _key(key)
{}

Channel::Channel(const Channel &other) : _name(other.getChannelName()), _topic(other.getTopic()), _op(other.getOp()), _clientLimit(other.getClientLimit()), _jointClients(other._jointClients), _invitedClients(other._invitedClients), _inviteOnly(other.isInviteOnly()), _hasKey(other.hasKey()), _topicRestricted(other.isTopicRestricted()), _key(other.getKey())
{}

Channel	&Channel::operator=(const Channel &other)
{
	if (this != &other)
	{
		_name = other.getChannelName();
		_topic = other.getTopic();
		_op = other.getOp();
		_clientLimit = other.getClientLimit();
		_jointClients = other._jointClients;
		_invitedClients = other._invitedClients;
		_inviteOnly = other.isInviteOnly();
		_hasKey = other.hasKey();
		_topicRestricted = other.isTopicRestricted();
		_key = other.getKey();
	}
	return (*this);
}

Channel::~Channel() {}

void Channel::removeClient(int cfd) {

	std::vector<int>::iterator ite = std::find(_jointClients.begin(), _jointClients.end(), cfd);

	if (ite != _jointClients.end()) {
		_jointClients.erase(ite);

		// decrement user count

		// should we delete channel if there is no member at all??
	} else {
		std::cerr << "Client FD not found << std::endl" << std::endl;
	}
}

void Channel::broadcast(const std::string& msg, int senderFd, bool excludeSender) {
	// so sender is an operator? How can I loop through the operator list?
	send(senderFd, msg.c_str(), msg.length(), 0); // send it to the operetor

	if (excludeSender) {
		for (size_t i = 0; i < _jointClients.size(); i++) {
			send(_jointClients[i], msg.c_str(), msg.length(), 0);
		}
	} else {
		for (size_t i = 0; i < _jointClients.size(); i++) {
			if (_jointClients[i] == senderFd) {
				i++;
			}
			send(_jointClients[i], msg.c_str(), msg.length(), 0);
		}
	}
}

void Channel::addToInvitedClients(int cfd) {
	for (size_t i = 0; i < _invitedClients.size(); i++) {
		if (_invitedClients[i] == cfd) {
			std::cerr << "User are already in the invited list" << std::endl;
		}
	}
	_invitedClients.push_back(cfd);
}



