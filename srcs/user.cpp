#include "../includes/user.hpp"

user::user(){
}

user::user(int fd, sockaddr_in address, serveur *serv):_address(address), _fd(fd), _serv(serv), _mode(""), _status("Unknown"), _invited(""){
    (void)_fd;
    (void)_address;
    command::init_func_map();
    // clear_terminal(fd);
}

user::~user(){
    _serv->delUser(_serv->_pollfds.begin()+_fd);
}

void user::parse_commands(std::string message){
    message = _buffito + message;
    _buffito = "";
    while (message.find("\r\n") != std::string::npos)
    {
        _commands.push_back(new command(message.substr(0, message.find("\r\n")), this));
        message = message.substr(message.find("\r\n")+2);
    }
    if (message.size() > 0)
        _buffito = message;
}

void user::execute_commands(){
    for (std::vector<command *>::iterator it = _commands.begin(); it != _commands.end(); it++)
    {
        (*it)->execute();
    }
    _commands.clear();
}

int user::isUser(std::string nick){
    for (std::map<int,user *>::iterator it = _serv->_users.begin(); it != _serv->_users.end(); it++)
    {
        if (it->second->_nick == nick)
            return 1;
    }
    return 0;
}


void clear_terminal(int fd)
{
    std::string message;

    message = "\x1B[2J\x1B[H";
    send(fd, message.c_str(), message.size(), 0);
    /*message = "\033[0;33mHello, Please enter the password with \033[0mPASS \"PASSWORLD\" \033[0;33m: \n\033[0m";
    send(fd, message.c_str(), message.size(), 0);*/
}
