#include "Channel.hpp"

Channel::Channel(std::string name, Client &op) : _name(name), _topic(""), _op(&op), _clientLimit(-1), _inviteOnly(false), _passRestricted(false), _topicRestricted(false), _password("")
{}

Channel::Channel(const Channel& other) : _name(other.getChannelName()), _topic(other.getTopic()), _op(other.getOp()), _clientLimit(other.getClientLimit()), _jointClients(other._jointClients), _invitedClients(other._invitedClients), _inviteOnly(other.isInviteOnly()), _passRestricted(other.isPassRestricted()), _topicRestricted(other.isTopicRestricted()), _password(other.getChannelPwd())
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
		_passRestricted = other.isPassRestricted();
		_topicRestricted = other.isTopicRestricted();
		_password = other.getChannelPwd();
	}
	return (*this);
}

Channel::~Channel() {}

std::vector<int>::iterator	Channel::getJointClients()
{
	std::vector<int>::iterator begin = _jointClients.begin();
	return begin;
}

std::vector<int>::iterator	Channel::getInvitedClients()
{
	std::vector<int>::iterator begin = _invitedClients.begin();
	return begin;
}