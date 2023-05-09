#include "command.hpp"

command::command(){
}
command::~command(){
}

command::command(std::string message, user *user):_user(user){
    while (message.find(" ") != std::string::npos)
    {
        _command.push_back(message.substr(0, message.find(" ")));
        message = message.substr(message.find(" ")+1);
    }
    if (message.size() > 0)
        _command.push_back(message);
    execute();
}

void command::execute(){
    if (!_command.size()){
        std::cout << "No command to execute" << std::endl;
        return ;
    }
    if (_user->_mode == 0){
        if (_command[0] != "NICK" && _command[0] != "USER" && _command[0] != "CAP" && _command[0] != "PASS" && _command[0] != "QUIT"){
            std::cout << "User not registered still need to set : " << (_user->_nick == "" ? "NICK " : "") << (_user->_realname == "" ? "USER " : "") << (_user->_mode == 0 ? "PASS " : "") << std::endl;
            return ;
        }
    }
    std::cout << "[" << _command[1] << "]" << std::endl;
    if (_command[0] == "PASS"){
        if (_command[1] != _user->_serv->_password){
            std::cout << "Wrong password" << std::endl;
            return ;
        }
        std::cout << "User registered : " << _user->_nick << std::endl;
        _user->_mode = 1;
        return ;
    }
    else if (_command[0] == "NICK"){
        _user->_nick = _command[1];
    }
    else if (_command[0] == "USER"){
        _user->_realname = _command[1];
    }
    else if (_command[0] == "JOIN"){
        if (_user->_serv->_channels.find(_command[1]) == _user->_serv->_channels.end()){
            _user->_serv->_channels[_command[1]] = new channel(_command[1], _user);
        }
    }
    else if (_command[0] == "QUIT"){
        exit(0);
    }
    else if (_command[0] == "PRINT"){
        std::cout << "User::nick = " << _user->_nick << "  User::realname = " << _user->_realname << std::endl;
    }
    else
        std::cout << _user->_nick << " : " << *this << std::endl;
}

std::ostream &operator<<(std::ostream &o, command &rhs){
    for (size_t i = 0; i < rhs._command.size(); i++)
    {
        o << rhs._command[i] << " ";
    }
    return (o);
}