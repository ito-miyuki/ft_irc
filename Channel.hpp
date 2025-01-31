#pragma once
#include "Client.hpp"

#include <algorithm>
#include <iostream> // for std::cerr, if you are not usinig it, delete it
#include <sys/socket.h> // for send

class Client;

class Channel {
    private:
        std::string			_name; // channel name
		std::string			_topic; // channel topic
		Client				*_op; // moderator client
		int					_clientLimit; // -1 for unlimited, positive int for limits
		std::vector<int>	_jointClients; // list of active users on the channel
		std::vector<int>	_invitedClients; // pending invitations for users
		bool				_inviteOnly; // for +i
		bool				_hasKey; // for +k, this could be unnecessary
		bool				_topicRestricted; // for +t
		std::string			_key; // channel password

    public:
        //Channel(); // this may be unnecessary
		Channel(std::string name, Client &op, std::string key);
        ~Channel();
        Channel(const Channel& other);
        Channel& operator=(const Channel& other);

		std::string			getChannelName() const {return _name;}
		std::string			getTopic() const {return _topic;}
		Client				*getOp() const {return _op;}
		int					getClientLimit() const {return _clientLimit;}
		std::vector<int>	&getJointClients() {return _jointClients;}
		std::vector<int>	&getInvitedClients() {return _invitedClients;}
		bool				isInviteOnly() const {return _inviteOnly;}
		bool				hasKey() const {return _hasKey;}
		bool				isTopicRestricted() const {return _topicRestricted;}
		std::string			getKey() const {return _key;}

		void				setChannelName(std::string name) {_name = name;}
		void				setTopic(std::string topic) {_topic = topic;}
		void				setOp(Client& op) {_op = &op;}
		void				setInviteOnly(bool status) {_inviteOnly = status;}
		void				setKeyStatus(bool status) {_hasKey = status;}
		void				setTopicRestricted(bool status) {_topicRestricted = status;}
		void				setKey(std::string key) {_key = key;}

		void				removeClient(int cfd);
		void				broadcast(const std::string& msg, int senderFd, bool excludeSender);
		void				addToInvitedClients(int cfd);
};