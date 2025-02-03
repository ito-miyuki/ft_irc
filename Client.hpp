#pragma once
#include <string>
#include <vector>
#include "Channel.hpp"

class Channel;

class Client {
    private:
        std::string             _password;
        std::string             _nickname;
        std::string             _username;
		std::string             _ipA;
        bool                    _isRegistered;
        int                     _fd; // fd for indentifying the matching poll struct
        std::vector<std::string>   _jointChannels; // list of channel names where user is part of
        std::vector<std::string>   _opChannels; // list of channel names where user is the moderator
    public:
		Client();
		Client(int fd, std::string ipAddress);
        ~Client();
        Client(const Client& other);
        Client& operator=(const Client& other);

		int			            getFd() const {return _fd;}
		bool		            isRegistered() const {return _isRegistered;}
		std::string         	getPassword() const {return _password;}
		std::string	            getNick() const {return _nickname;}
		std::string	            getUser() const {return _username;}
		std::string	            getIPa() const {return _ipA;}
        std::vector<std::string>   &getJointChannels() {return _jointChannels;}
        std::vector<std::string>   &getOpChannels() {return _opChannels;}

		void                    setAsRegistered() {_isRegistered = true;}
        void                    setPassword(std::string password) {_password = password;}
        void                    setNickname(std::string nick) {_nickname = nick;}
        void                    setUsername(std::string user) {_username = user;}
        void                    setFd(int fd) {_fd = fd;}
		void					addChannel(std::string channelName) {_jointChannels.push_back(channelName);}
		void					addOpChannel(std::string channelName) {_opChannels.push_back(channelName);}

		void					removeChannel(std::string channelName);
		void					removeOpChannel(std::string channelName);
		void					clearAllChannels();
};