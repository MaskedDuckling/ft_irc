#include "user.hpp"

user::user(){
}

user::user(int fd, sockaddr_in address, serveur *serv):_address(address), _fd(fd), _serv(serv), _mode(0){
    (void)_fd;
    (void)_address;
}

user::~user(){
}

void user::parse_commands(std::string message){
    while (message.find("\n") != std::string::npos)
    {
        _commands.push_back(new command(message.substr(0, message.find("\n")), this));
        message = message.substr(message.find("\n")+1);
    }
    if (message.size() > 0)
        _commands.push_back(new command(message, this));
}

void user::execute_commands(){
    for (std::vector<command *>::iterator it = _commands.begin(); it != _commands.end(); it++)
    {
        (*it)->execute();
    }
    _commands.clear();
}