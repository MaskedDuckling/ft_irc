#include "user.hpp"

user::user(){
}

user::user(int fd, sockaddr_in address, serveur *serv):_address(address), _fd(fd), _serv(serv), _mode(0){
    (void)_fd;
    (void)_address;
}

user::~user(){
    _serv->delUser(_serv->_pollfds.begin()+_fd);
}

void user::parse_commands(std::string message){
    while (message.find("\r\n") != std::string::npos)
    {
        _commands.push_back(new command(message.substr(0, message.find("\r\n")), this));
        message = message.substr(message.find("\r\n")+2);
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