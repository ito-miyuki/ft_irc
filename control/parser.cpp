#include "../Server.hpp"

void    Server::parser(std::string arg, std::vector<std::string> &params, char del)
{
    std::stringstream	ss(arg);
    std::string			substr;

    while (getline(ss, substr, del))
    {
        params.push_back(substr);
    }
}
