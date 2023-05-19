#include "user.hpp"

user::user(){
}

user::user(int fd, sockaddr_in address, serveur *serv):_address(address), _fd(fd), _serv(serv), _mode(""), _status("Unknown"){
    (void)_fd;
    (void)_address;
    clear_terminal(fd);
}

user::~user(){
    _serv->delUser(_serv->_pollfds.begin()+_fd);
}

int user::add_mode(std::string mode){
    std::string available_modes("aiwroOs");
    if (available_modes.find(mode) == std::string::npos)
        return 0;
    else if (_mode.find(mode) == std::string::npos)
        _mode += mode;
    return 1;
}
int user::deleteUserMode(char newMode){
	size_t n;

	n = _mode.find(newMode);
	if (n != std::string::npos){
		_mode.erase(n, 1);
        return (0);
    }
    return (1);
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

void clear_terminal(int fd)
{
    std::string message;

    message = "\x1B[2J\x1B[H";
    send(fd, message.c_str(), message.size(), 0);
    message = "\033[0;33mHello, Please enter the password with \033[0mPASS <PASSWORLD> \033[0;33m: \033[0m";
    send(fd, message.c_str(), message.size(), 0);
}