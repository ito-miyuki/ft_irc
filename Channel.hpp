#pragma once
# include "Client.hpp"
# include <algorithm>

# include "Server.hpp"

class Client;

class Channel {
    private:
        std::string			_name; // channel name
		std::string			_topic; // channel topic
		std::vector<int>	_ops; // moderator clients
		int					_clientLimit; // -1 for unlimited, positive int for limits
		std::vector<int>	_jointClients; // list of active users on the channel
		std::vector<int>	_invitedClients; // pending invitations for users
		bool				_inviteOnly; // for +i
		bool				_topicRestricted; // for +t
		std::string			_key; // channel password
		std::time_t			_topicUpdated;
		std::string			_editer;

    public:
        Channel();
		Channel(std::string name, int op, std::string key);
        ~Channel();
        Channel(const Channel& other);
        Channel& operator=(const Channel& other);

		std::string			getChannelName() const {return _name;}
		std::string			getTopic() const {return _topic;}
		std::vector<int>	&getOps() {return _ops;}
		int					getClientLimit() const {return _clientLimit;}
		std::vector<int>	&getJointClients() {return _jointClients;}
		std::vector<int>	&getInvitedClients() {return _invitedClients;}
		bool				isInviteOnly() const {return _inviteOnly;}
		bool				isTopicRestricted() const {return _topicRestricted;}
		std::string			getKey() const {return _key;}

		void				setChannelName(std::string name) {_name = name;}
		void				setTopic(std::string topic) {_topic = topic;}
		void				addOp(int op) {_ops.push_back(op);}
		void				setInviteOnly(bool status) {_inviteOnly = status;}
		void				setTopicRestricted(bool status) {_topicRestricted = status;}
		void				setKey(std::string key) {_key = key;}
		void				setClientLimit(int limit) {_clientLimit = limit;}

		bool				containSender(int cfd) const;

		void				removeClient(int cfd);
		void				addToInvitedClients(int cfd);
		void				removeOp(int cfd);
		void				addClient(int cfd) {_jointClients.push_back(cfd);}
		void				removeInvite(int cfd);

		void				broadcast(const std::string& msg, int senderFd, bool excludeSender);

		void				setEditer(std::string editor) {_editer = editor;}
		std::string			getEditer() {return (_editer);}

		void				setTopicUpdated(std::time_t update) {_topicUpdated = update;}
		std::time_t			getTopicUpdated() {return (_topicUpdated);}
};