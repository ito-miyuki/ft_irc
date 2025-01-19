#pragma once
#include "Client.hpp"

class Channel {
    private:
        std::string			_name; // channel name
		std::string			_topic; // channel topic
		Client				*_op; // moderator client
		int					_clientLimit; // -1 for unlimited, positive int for limits
		std::vector<int>	_jointClients; // list of active users on the channel
		std::vector<int>	_invitedClients; // pending invitations for users
		bool				_inviteOnly; // for +i
		bool				_passRestricted; // for +k, this could be unnecessary
		bool				_topicRestricted; // for +t
		std::string			_password; // channel password

    public:
        //Channel(); // this may be unnecessary
		Channel(std::string name, Client &op);
        ~Channel();
        Channel(const Channel& other);
        Channel& operator=(const Channel& other);

		std::string					getChannelName() const {return _name;}
		std::string					getTopic() const {return _topic;}
		Client						*getOp() const {return _op;}
		int							getClientLimit() const {return _clientLimit;}
		std::vector<int>::iterator	getJointClients();
		std::vector<int>::iterator	getInvitedClients();
		bool						isInviteOnly() const {return _inviteOnly;}
		bool						isPassRestricted() const {return _passRestricted;}
		bool						isTopicRestricted() const {return _topicRestricted;}
		std::string					getChannelPwd() const {return _password;}

		void						setChannelName(std::string name) {_name = name;}
		void						setTopic(std::string topic) {_topic = topic;}
		void						setOp(Client& op) {_op = &op;}
		void						setInviteOnly(bool status) {_inviteOnly = status;}
		void						setPassRestricted(bool status) {_passRestricted = status;}
		void						setTopicRestricted(bool status) {_topicRestricted = status;}
		void						setChannelPwd(std::string password) {_password = password;}

};