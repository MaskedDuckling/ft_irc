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

void command::execute(){
    if (!_command.size()){
        std::cout << "No command to execute" << std::endl;
        return ;
    }
    std::cout << "Executing command : {" << *this <<"}"<< std::endl;
    std::cout << _command[0] << std::endl;
    // if (_user->_mode == 0){
    //     if (_command[0] != "NICK" && _command[0] != "USER" && _command[0] != "CAP" && _command[0] != "PASS" && _command[0] != "QUIT"){
    //         std::cout << "User not registered still need to set : " << (_user->_nick == "" ? "NICK " : "") << (_user->_realname == "" ? "USER " : "") << (_user->_mode == 0 ? "PASS " : "") << std::endl;
    //         return ;
    //     }
    //     if (_command[0] == "CAP")
    //         return ;
    // }
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
    else if (_user->_mode == 2){
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