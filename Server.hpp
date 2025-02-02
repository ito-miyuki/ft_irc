#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <poll.h> // for pol()
#include <sys/socket.h> // for socket() and send
#include <netinet/in.h> // for sockaddr_in
#include <unistd.h> // for close()
#include <cstring> // for memset
#include <fcntl.h>
#include <arpa/inet.h> // for inet_ntoa()
#include <sstream>
#include <regex>
#include <csignal>
#include <algorithm>

#define MAX_CLIENTS 100
#define MAX_CHANNELS 100

#include "Client.hpp"
#include "Channel.hpp"

class Channel;
class Client;

class Server {
    private:
        int 					_port; // do we want to name like this instead? port_ 
        std::string 			_password;
        int 					_serverFd; // not in use yet
        std::vector<Client>		_clients; // client's info
        std::vector<Channel>	_channels; // channel info
		std::vector<pollfd>		_fds;
		size_t					_clientAmount;
		static bool				_signal; // static is accessable without instance

		enum	Command {
				KICK,
				INVITE,
				TOPIC,
				MODE,
				JOIN,
				PRIVMSG,
				NICK,
				PING,
				QUIT,
				FAIL
		};

		void	parser(std::string arg, std::vector<std::string> &params);
		bool	hasOpRights(int cfd, std::string channelName);

		void	acceptNewClient();
		void	processClientInput(size_t *clientIndex, int cfd);
		bool	isRegistered(int cfd);
		void	registerClient(int cfd, std::string arg);
		void	processInputData(std::stringstream &ss, int cfd);
		void	authenticate(Client &client, std::string arg);

		void	registerPassword(Client& client, std::string arg);
        void	registerNickname(Client& client, std::string arg);
        void    registerUser(Client& client, std::string arg);

		void	eraseClient(int cfd);
		bool	isUniqueNick(std::string nick);

		void	runCommand(int cfd, std::string arg);
		void	nick(int cfd, std::string arg);
		void	verifyNick(int cfd, std::string newNick);
		void	join(int cfd, std::string arg);
		std::string		parseChannelInfo(std::string channels);
		void	addNewChannel(int cfd, std::string channelName, std::string channelKey);
		void	joinChannel(int cfd, std::vector<std::string> &params);
		bool	isInvited(int cfd, std::vector<int> &invitedClients);
		bool	alreadyJoint(int cfd, std::vector<int> &jointClients);
		bool	checkKey(int cfd, Channel &channel, std::string key);
		bool	checkInvite(int cfd, Channel &channel);
		bool	checkLimit(int cfd, Channel &channel);
		void	welcomeClient(int cfd, Channel &channel, Client &client);
		void	leaveAllChannels(int cfd);

		void	mode(int cfd, std::string arg);
		void	setMode(int cfd, std::vector<std::string> &params);
		bool	verifyParams(int cfd, std::vector<std::string> &params);
		void	setInviteStatus(int cfd, Channel &channel, std::string mode);
		void	setTopicRestriction(int cfd, Channel &channel, std::string mode);
		void	setKey(int cfd, Channel &channel, std::vector<std::string> &params);
		void	setClientLimit(int cfd, Channel &channel, std::vector<std::string> &params);
		void	setOpRights(int cfd, std::vector<std::string> &params);
		bool	isClient(std::string nick);
		bool	getClient(std::string name, Client *client);
		Channel	*findChannel(Client &op, Client &newOp);

		void	pingMyPong(int cfd, std::string arg);
		Server::Command	identifyCommand(std::string command);

		void	kickSomeone(int cfd, std::string arg);
		bool	channelExist(const std::string& channelName); // should this be in Channel class?
		bool	isUserInChannel(const std::string& userName, const std::string& channelName, int userFd); // should this be in other class?
		int		getUserFdByNick(const std::string& nickName); // should this be in other class?
		Channel* getChannelObj(const std::string& channelName); // temporary solution

		Client* getClientObjByFd(int fd);

		void	inviteRandos(int cfd, std::string arg);
		// bool	nicknameExist(const std::string& nickname);
		int		getUserFdbyNick(const std::string& nickName);
		int		getClientIndex(int fd);
		int		getChannelIndex(std::string name);

    public:
        Server(int port, std::string password); // should it be?: const std::string& password
        ~Server();
        Server(const Server& other);
        Server& operator=(const Server& other);

        int		runServer();
		void	setServerFd();
        void	incrementClientAmount() {_clientAmount++;}
        void	decrementClientAmount();

        std::string	getPassword() const {return _password;}
        int			getPort() const {return _port;}
        int			getServerFd() const {return _serverFd;}
        size_t 		getClientAmount() const {return _clientAmount;}

		void		addClient(const Client &client) {_clients.push_back(client);}
		void		addChannel(const Channel &channel) {_channels.push_back(channel);}

		static void setSignal(bool value); // is this a correct place to put?
};