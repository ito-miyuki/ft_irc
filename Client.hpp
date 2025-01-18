#pragma once
#include <string>
#include <vector>
//#include "Channel.hpp"

class Client {
    private:
        std::string _password;
        std::string _nickname;
        std::string _username;
		std::string _ipA;
        bool _isRegistered;
        int _fd; // fd for indentifying the matching poll struct
        // std::vector<Channel> jointChannels; // channels user is part of
        // std::vector<Channel> opChannels; // channels where user is the moderator
    public:
		Client(int fd, std::string ipAddress);
        ~Client();
        Client(const Client& other);
        Client& operator=(const Client& other);

		int			getFd() {return _fd;}
		bool		isRegistered() {return _isRegistered;}
		std::string	getPassword() {return _password;}
		std::string	getNick() {return _nickname;}
		std::string	getUser() {return _username;}
		void		setAsRegistered() {_isRegistered = true;}

        void        setPassword(std::string password) {_password = password;}
        void        setNickname(std::string nick) {_nickname = nick;}
        void        setUsername(std::string user) {_username = user;}
        void        setFd(int fd) {_fd = fd;}

};