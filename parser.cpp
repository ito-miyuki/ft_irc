#include "Server.hpp"

void    Server::parser(std::string arg, std::vector<std::string> &params)
{
    std::stringstream	ss(arg);
    std::string			substr;
    char				del = ' ';
    int					argCount = 0;

    while (getline(ss, substr, del))
    {
        if (argCount != 0)
            params.push_back(substr);
        argCount++;
    }
}
