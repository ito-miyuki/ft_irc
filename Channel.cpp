#include "Channel.hpp"

Channel::Channel(std::string name, Client &op) : _name(name), _op(&op), clientLimit(-1), _inviteOnly(false), _passRestricted(false), _topicRestricted(false)
{}

Channel::~Channel() {}

//add getters and setters...