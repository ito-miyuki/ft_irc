#include "Server.hpp"

//:sataskin!~sataskin@194.136.126.51 TOPIC #topictester :cat

void    Server::topic(int cfd, std::string arg)
{
    std::cout << "this is our arg: " << arg << "| and this is our cfd: " << cfd << std::endl;
}