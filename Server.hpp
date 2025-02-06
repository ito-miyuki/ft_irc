#pragma once
# include <string>
# include <vector>
# include <iostream>
# include <poll.h> // for pol()
# include <sys/socket.h> // for socket() and send
# include <netinet/in.h> // for sockaddr_in
# include <unistd.h> // for close()
# include <cstring> // for memset
# include <fcntl.h>
# include <arpa/inet.h> // for inet_ntoa()
# include <sstream>
# include <regex>
# include <csignal>
# include <algorithm>
# include <ctime>
# include <map>

# define MAX_CLIENTS 20

# include "Client.hpp"
# include "Channel.hpp"

class Channel;
class Client;

class Server {
    private:
        int 						_port;
        std::string 				_password;
        int 						_serverFd;
        std::vector<Client>			_clients;
        std::vector<Channel>		_channels;
		std::vector<pollfd>			_fds;
		static bool					_signal; // static is accessable without instance
		std::map<int, std::string>	_clientBuffers;

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

		/*		GENERAL						*/

		void			parser(std::string arg, std::vector<std::string> &params, char del);
		bool			hasOpRights(int cfd, std::string channelName);
		void			runCommand(int cfd, std::string arg, size_t *clientIndex);
		bool			isUniqueNick(std::string nick);
		Server::Command	identifyCommand(std::string *cmd);


		/*		CLIENT REGISTRATION			*/

		void	acceptNewClient();
		void	processClientInput(size_t *clientIndex, int cfd);
		bool	isRegistered(int cfd);
		void	registerClient(int cfd, std::string arg, size_t *clientIndex);
		void	processInputData(std::stringstream &ss, int cfd, size_t *clientIndex);
		void	authenticate(Client &client, std::string arg, size_t *clientIndex);
		void	registerPassword(Client& client, std::string arg, size_t *clientIndex);
        void	registerNickname(Client& client, std::string arg);
        void    registerUser(Client& client, std::string arg);

		/*		JOIN						*/

		void	join(int cfd, std::string arg);
		void	addNewChannel(int cfd, std::string channelName, std::string channelKey);
		void	joinChannel(int cfd, std::string channelName, std::string channelKey);
		bool	isInvited(int cfd, std::vector<int> &invitedClients);
		bool	alreadyJoint(int cfd, std::vector<int> &jointClients);
		bool	checkKey(int cfd, Channel &channel, std::string key);
		bool	checkInvite(int cfd, Channel &channel);
		bool	checkLimit(int cfd, Channel &channel);
		void	welcomeClient(int cfd, Channel &channel, Client &client);
		bool	isValidName(std::string channel);
		bool	isValidKey(std::string channelKey);

		/*		NICK						*/

		void	nick(int cfd, std::string arg);
		void	verifyNick(int cfd, std::string newNick);

		/*		MODE						*/

		void		mode(int cfd, std::string arg);
		void		setMode(int cfd, std::vector<std::string> &params);
		bool		verifyParams(int cfd, std::vector<std::string> &params);
		void		setInviteStatus(int cfd, Channel &channel, std::string mode);
		void		setTopicRestriction(int cfd, Channel &channel, std::string mode);
		void		setKey(int cfd, Channel &channel, std::vector<std::string> &params);
		void		setClientLimit(int cfd, Channel &channel, std::vector<std::string> &params);
		void		setOpRights(int cfd, std::vector<std::string> &params);
		bool		isClient(std::string nick);
		bool		getClient(std::string name, Client *client);
		void		returnChannelMode(int cfd, Channel &channel);
		std::string	findChannel(Client &op, Client &newOp);
		std::string	findCommonChannel(int cfd, std::string targetNick);

		/*		PING						*/

		void	pingMyPong(int cfd, std::string arg);

		/*		PRIVMSG						*/

		void	messages(int cfd, std::string arg);
		void    sendToClient(int cfd, std::string dm, std::string recipient);
		void    sendToChannel(int cfd, std::string dm, std::string recipient);
		bool    checkClient(std::string name, Client *client);
		bool    checkChannel(std::string name, Channel *channel);
		bool    checkSender(int cfd, Client *client);

		/*		KICK						*/

		void		kickSomeone(int cfd, std::string arg);
		bool		channelExist(const std::string& channelName);
		bool		isUserInChannel(const std::string& channelName, int userFd);
		int			getUserFdByNick(const std::string& nickName);
		Channel*	getChannelObj(const std::string& channelName); 
		Client*		getClientObjByFd(int fd);

		/*		INVITE						*/

		void	inviteRandos(int cfd, std::string arg);
		int		getUserFdbyNick(const std::string& nickName);
		int		getClientIndex(int fd);
		int		getChannelIndex(std::string name);

		/*		TOPIC						*/

		void    topic(int cfd, std::string arg);
		void    changeTopic(Channel &channel, int cfd, std::string newTopic);
		void	notifyChannels(int cfd, std::string msg);
		void	removeDeadChannels();

		/*		QUIT						*/

		void	quit(int cfd, std::string arg, size_t *clientIndex);
		void	eraseClient(int cfd, size_t *clientIndex);
		void	removeClientFromChannels(int cfd);

    public:
        Server(int port, std::string password);
        ~Server();
        Server(const Server& other);
        Server& operator=(const Server& other);

        int			runServer();
		void		setServerFd();

        std::string	getPassword() const {return _password;}
        int			getPort() const {return _port;}
        int			getServerFd() const {return _serverFd;}

		void		addClient(const Client &client) {_clients.push_back(client);}
		void		addChannel(const Channel &channel) {_channels.push_back(channel);}

		static void setSignal(bool value);

};