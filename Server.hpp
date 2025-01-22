#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <poll.h> // for pol()
#include <sys/socket.h> // for socket()
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
		void	parseChannelInfo(int cfd, std::string channelNames, std::string keys);
		void	verifyChannels(int cfd, std::vector<std::string> &channels, std::vector<std::string> &keys);
		bool	isValidName(std::string channel);
		void	joinChannels(int cfd, std::vector<std::string> &channels, std::vector<std::string> &keys);
		bool	isInvited(int cfd, std::vector<int> &invitedClients);
		bool	alreadyJoint(int cfd, std::vector<int> &jointClients);
		void	checkKey(int cfd, Channel &channel, std::vector<std::string> &keys, bool *canJoin, int index);
		void	checkInvite(int cfd, Channel &channel, bool *canJoin);
		void	checkLimit(int cfd, Channel &channel, bool *canJoin);
		void	welcomeClient(int cfd, Channel &channel, int channelAmount);

		void	pingMyPong(int cfd, std::string arg);
		Server::Command	identifyCommand(std::string command);

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

		Channel	&getChannel(std::string name);
		Client	&getClient(int fd);


		static void setSignal(bool value); // is this a correct place to put?
};