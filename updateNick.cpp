#include "Server.hpp"

void	Server::updateNick(int cfd, std::string arg)
{
	std::stringstream	ss(arg);
	std::string			substr;
	char				del = ' ';
	int					argCount = 0;
	std::string			newNick;

	while (getline(ss, substr, del))
	{
		if (argCount > 0)
			newNick = substr;
		argCount++;
	}
	if (argCount == 1)
	// issue ERR_NONICKNAMEGIVEN and print out users current nick
	else
	{
		//regex verigy nickname ERR_ERRONEUSNICKNAME
		// check that it's not in use yet ERR_NICKNAMEINUSE
	} 
}
