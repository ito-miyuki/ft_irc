#include "Channel.hpp"

Channel::Channel() : _name(""), _topic(""), _clientLimit(-1), _inviteOnly(false), _topicRestricted(true), _key("")
{}

Channel::Channel(std::string name, int op, std::string key) : _name(name), _topic(""), _clientLimit(-1), _inviteOnly(false), _topicRestricted(true), _key(key)
{
	_ops.push_back(op);
}

Channel::Channel(const Channel &other) : _name(other.getChannelName()), _topic(other.getTopic()), _ops(other._ops), _clientLimit(other.getClientLimit()), _jointClients(other._jointClients), _invitedClients(other._invitedClients), _inviteOnly(other.isInviteOnly()), _topicRestricted(other.isTopicRestricted()), _key(other.getKey())
{}

Channel	&Channel::operator=(const Channel &other)
{
	if (this != &other)
	{
		_name = other.getChannelName();
		_topic = other.getTopic();
		_ops = other._ops;
		_clientLimit = other.getClientLimit();
		_jointClients = other._jointClients;
		_invitedClients = other._invitedClients;
		_inviteOnly = other.isInviteOnly();
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
    }
}

void Channel::removeOp(int cfd) {

    std::vector<int>::iterator ite = std::find(_ops.begin(), _ops.end(), cfd);

    if (ite != _ops.end()) {
        _ops.erase(ite);
    }
}

void	Channel::removeInvite(int cfd) {

	std::vector<int>::iterator ite = std::find(_invitedClients.begin(), _invitedClients.end(), cfd);

    if (ite != _invitedClients.end()) {
        _invitedClients.erase(ite);
    }
}

void Channel::broadcast(const std::string& msg, int senderFd, bool excludeSender) {

    if (excludeSender) {
		for (size_t i = 0; i < _ops.size(); i++) {
            send(_ops[i], msg.c_str(), msg.length(), 0);
        }
        for (size_t i = 0; i < _jointClients.size(); i++) {
            send(_jointClients[i], msg.c_str(), msg.length(), 0);
        }
    } else {
		for (size_t i = 0; i < _ops.size(); i++) {
            if (_ops[i] == senderFd) {
                i++;
            }
            send(_ops[i], msg.c_str(), msg.length(), 0);
        }
        for (size_t i = 0; i < _jointClients.size(); i++) {
            if (_jointClients[i] == senderFd) {
                i++;
            }
            send(_jointClients[i], msg.c_str(), msg.length(), 0);
        }
    }
}

bool	Channel::containSender(int cfd) const 
{
    if (std::find(_jointClients.begin(), _jointClients.end(), cfd) != _jointClients.end())
        return (true);
    if (std::find(_ops.begin(), _ops.end(), cfd) != _ops.end())
        return (true);
    return (false);
}
