#include "Channel.hpp"

Channel::Channel() : _name(""), _topic(""), _op(nullptr), _clientLimit(-1), _inviteOnly(false), _topicRestricted(false), _key("")
{}

Channel::Channel(std::string name, Client &op, std::string key) : _name(name), _topic(""), _op(&op), _clientLimit(-1), _inviteOnly(false), _topicRestricted(false), _key(key)
{}

Channel::Channel(const Channel &other) : _name(other.getChannelName()), _topic(other.getTopic()), _op(other.getOp()), _clientLimit(other.getClientLimit()), _jointClients(other._jointClients), _invitedClients(other._invitedClients), _inviteOnly(other.isInviteOnly()), _topicRestricted(other.isTopicRestricted()), _key(other.getKey())
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
		_topicRestricted = other.isTopicRestricted();
		_key = other.getKey();
	}
	return (*this);
}

Channel::~Channel() {}
