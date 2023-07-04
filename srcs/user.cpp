#include "../includes/user.hpp"

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

int user::add_mode(std::string mode, int oper){
    std::string available_modes("aiwroOs");
    if (available_modes.find(mode) == std::string::npos)
        return 0;
    else if (_mode.find(mode) == std::string::npos)
    {
        std::string str;
        if (mode == "o" && oper == 1)
            _mode += mode;
        else if(mode == "o" && oper == 0)
            str = "\033[0;31mCannot add operator, use cmd OPER\n\033[0m";
        else if (mode == "O" && oper == 1)
            _mode += mode;
        else if(mode == "O" && oper == 0)
            str = "\033[0;31mCannot add operator, use cmd OPER\n\033[0m";
        else if (mode == "a")
        {
            _mode += mode;
            str = "\033[0;32mYou are now tag as away\n\033[0m";
            send(_fd, mode.c_str(), mode.size(), 0);
        }
        else if (mode == "i")
        {
            _mode += mode;
            str = "\033[0;32mYou are now invisible\n\033[0m";
            send(_fd, mode.c_str(), mode.size(), 0);
        }
        else if (mode == "w")
        {
            _mode += mode;
            str = "\033[0;32mReceive wallops messages!\n\033[0m";
            send(_fd, mode.c_str(), mode.size(), 0);
        }
        else if (mode == "r")
        {
            _mode += mode;
            str = "\033[0;32mRestricted user connection\n\033[0m";
            send(_fd, mode.c_str(), mode.size(), 0);
        }
        else if (mode == "s")
        {
            _mode += mode;
            str = "\033[0;32mYou can receive server notice\n\033[0m";
            send(_fd, mode.c_str(), mode.size(), 0);
        }
    }
    return 1;
}
int user::deleteUserMode(char newMode){
	size_t n;

	n = _mode.find(newMode);
	if (n != std::string::npos)
    {
		_mode.erase(n, 1);
        std::string str;
        if (newMode == 'a')
        {
            str = str = "\033[0;32mYou are no longer tag as away\n\033[0m";
            send(_fd, str.c_str(), str.size(), 0);
        }
        else if (newMode == 'i')
        {
            str = str = "\033[0;32mYou are no longer invisible\n\033[0m";
            send(_fd, str.c_str(), str.size(), 0);
        }
        else if (newMode == 'w')
        {
            str = "\033[0;32mno longer Receive wallops messages!\n\033[0m";
            send(_fd, str.c_str(), str.size(), 0);
        }
        else if (newMode == 'r')
        {
            str = "\033[0;32mdelete Restricted user connection\n\033[0m";
            send(_fd, str.c_str(), str.size(), 0);
        }
        else if (newMode == 'o')
        {
            str = "\033[0;32myou are no longer an operator\n\033[0m";
            send(_fd, str.c_str(), str.size(), 0);
        }
        else if (newMode == 'O')
        {
            str = "\033[0;32myou are no longer a local operator\n\033[0m";
            send(_fd, str.c_str(), str.size(), 0);
        }
        else if (newMode == 's')
        {
            str = "\033[0;32mYou can no longer receive server notice\n\033[0m";
            send(_fd, str.c_str(), str.size(), 0);
        }
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
    /*message = "\033[0;33mHello, Please enter the password with \033[0mPASS \"PASSWORLD\" \033[0;33m: \n\033[0m";
    send(fd, message.c_str(), message.size(), 0);*/
}
