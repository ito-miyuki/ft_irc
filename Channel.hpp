#pragma once
#include "Client.hpp"

class Channel {
    private:
        std::string			_name; // channel name
		std::string			_topic; // channel topic
		Client				*_op; // moderator client
		int					clientLimit; // -1 for unlimited, positive int for limits
		std::vector<Client>	_jointClients; // list of active users on the channel
		std::vector<Client>	_invitedClients; // pending invitations for users
		bool				_inviteOnly; // for +i
		bool				_passRestricted; // for +k, this could be unnecessary
		bool				_topicRestricted; // for +t
		std::string			_password; // channel password

    public:
        //Channel(); // this may be unnecessary
		Channel(std::string name, Client &op);
        ~Channel();
        //Channel(const Channel& other); // is this really needed tho...?
        //Channel& operator=(const Channel& other); // also, do we really need this?
};