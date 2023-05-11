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
    if (message.size() > 0){
        _command.push_back(message.substr(0, message.find("\n")));
        message = message.substr(message.find("\n")+1);
    }
}

void command::execute(){                                                                /*A refaire entieremment*/
    if (!_command.size()){
        std::cout << "No command to execute" << std::endl;
        return ;
    }
    std::cout << "Executing command : {" << *this <<"}"<< std::endl;
    if (_command[0] == "PASS"){
        if (_command[1] != _user->_serv->_password){
            std::cout << "Wrong password [" << _command[1] << "]"<< std::endl;
            return ;
        }
        if (_user->_nick=="" || _user->_realname=="")
            return ;
        std::cout << "User registered : " << _user->_nick << std::endl;
        _user->_mode = 1;
    }
    else if (_command[0] == "NICK"){
        _user->_nick = _command[1];
    }
    else if (_command[0] == "USER"){
        _user->_realname = _command[1];
        std::string rep("001 Welcome to the Internet Relay Network ");
        rep +=_user->_nick + "!" + _user->_realname + "@" + _user->_serv->_name + "\n";
        send(_user->_fd, rep.c_str(), rep.size(), 0);
    }
    else if (_command[0] == "QUIT"){
        _user->~user();
    }
    else if (_command[0] == "JOIN"){
        if (_user->_serv->_channels[_command[1]])
            _user->_serv->_channels[_command[1]]->add_user(_user);
        else
            _user->_serv->_channels[_command[1]] = new channel(_command[1], _user);
    }
    else if (_command[0] == "MODE"){
        return;
    }
    else if (_user->_mode == 2 || _command[0] == "PRIVMSG"){
        std::string response = _user->_nick + " : ";
        for (std::vector<std::string>::iterator it = _command.begin(); it != _command.end(); it++)
            response += *it + " ";
        response += "\n";
        _user->_channel->broadcast(response);
    }
}

std::ostream &operator<<(std::ostream &o, command &rhs){                                                                    /*FAIT BUGGER L'AFFICHAGE*/
    for(std::vector<std::string>::iterator it = rhs._command.begin(); it != rhs._command.end(); it++){
        o << *it;
        if (it != rhs._command.end()-1)
            std::cout << " ";
    }
    return (o);
}