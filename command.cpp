#include "command.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

std::map<std::string, void (command::*)()> command::_map_fonction;

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
    init_func_map();
    if (!_command.size()){
        std::cout << "No command to execute" << std::endl;
        return ;
    }
    std::cout << "Executing command: " << *this << std::endl;

    if (_map_fonction.find(_command[0]) != _map_fonction.end())
            (this->*_map_fonction[_command[0]])();
    else if (_command[0] == "CAP"){}
    else
        std::cout << "Command not found" << std::endl;
}

void command::display_reply(std::string reply, ...){        /*Probablement erreur ici*/
    std::string message;
    va_list arg_list;
    va_start(arg_list,reply);
    for (std::string::iterator it = reply.begin() ; it != reply.end(); it++){
        if (*it!='<')
            message += *it;
        else{
            while (it != reply.end() && *it != '>')
                it++;
            message += va_arg(arg_list, char *);
        }
    }
    message += "\r\n";
    std::cout << message;
    send(_user->_fd, message.c_str(), message.size(), 0);
    va_end(arg_list);
}

void command::init_func_map(){
    _map_fonction.insert(std::make_pair("PASS",&command::PASS));
    _map_fonction.insert(std::make_pair("USER",&command::USER));
    _map_fonction.insert(std::make_pair("NICK",&command::NICK));

    _map_fonction.insert(std::make_pair("JOIN",&command::JOIN));
    _map_fonction.insert(std::make_pair("OPER",&command::OPER));
    _map_fonction.insert(std::make_pair("MODE",&command::MODE));
    _map_fonction.insert(std::make_pair("WHOIS",&command::WHOIS));


    _map_fonction.insert(std::make_pair("PART",&command::PART));
    _map_fonction.insert(std::make_pair("QUIT",&command::QUIT));

    _map_fonction.insert(std::make_pair("PING",&command::PING));
    _map_fonction.insert(std::make_pair("PONG",&command::PONG));

}



void command::PASS(){
    if (_command.size() < 2)
        return display_reply(ERR_NEEDMOREPARAMS, "PASS");
    if (_user->_status == "Registered")
        return display_reply(ERR_ALREADYREGISTRED);
    if (_user->_serv->_password != _command[1])
        return display_reply(ERR_PASSWDMISMATCH);
    _user->_status = "Registered";
    _user->_password = _command[1];
    return display_reply("\033[0;32mPassword correct !\033[0m, enter your nickname with ");
}
void command::USER(){
    if (_command.size() < 5)
        return display_reply(ERR_NEEDMOREPARAMS, "USER");
    if (_user->_status != "Registered")
        return display_reply(ERR_ALREADYREGISTRED);
    _user->_realname = _command[1];

    if (_user->_nick != ""){
        std::string tmp = inet_ntoa(_user->_address.sin_addr);
        display_reply(CLEAR_TERM);
        display_reply(RPL_WELCOME, _user->_nick.c_str(), _user->_realname.c_str(), tmp.c_str());
		display_reply(RPL_YOURHOST, _user->_serv->_name.c_str(), "1.0");
		display_reply(RPL_CREATED, "today");
		display_reply(RPL_MYINFO, _user->_serv->_name.c_str(), "1.0", "*", "*");
        _user->_status = "Connected";
    }
}
void command::NICK(){
    if (_command.size() < 2)
        return display_reply(ERR_NONICKNAMEGIVEN);
    if (_user->_mode.find('r') != std::string::npos)
        return display_reply(ERR_RESTRICTED);
    for (std::map<int, user *>::iterator it = _user->_serv->_users.begin(); it != _user->_serv->_users.end(); it++)
        if (it->second->_nick == _command[1])
            return display_reply(ERR_NICKNAMEINUSE, _command[1].c_str());
    // if (kekchose)
    //     return display_reply(ERR_ERRONEUSNICKNAME, _command[1].c_str());
    // if (kekchose d'autre)
    //     return display_reply(ERR_UNAVAILRESOURCE, _command[1].c_str());
    _user->_nick = _command[1];

    if (_user->_realname != ""){
        std::string tmp = inet_ntoa(_user->_address.sin_addr);
        clear_terminal(_user->_fd);
        display_reply(RPL_WELCOME, _user->_nick.c_str(), _user->_realname.c_str(), tmp.c_str());
		display_reply(RPL_YOURHOST, _user->_serv->_name.c_str(), "1.0");
		display_reply(RPL_CREATED, "today");
		display_reply(RPL_MYINFO, _user->_serv->_name.c_str(), "1.0", "*", "*");
        _user->_status = "Connected";
    }
}
void command::JOIN(){}
void command::OPER(){
    if (_command.size() < 3)
        return display_reply(ERR_NEEDMOREPARAMS, _command[0].c_str());
    if (_command[1] != _user->_nick)
        return display_reply(ERR_ERRONEUSNICKNAME, _command[1].c_str());
    if (_command[2] != _user->_password)
        return display_reply(ERR_PASSWDMISMATCH);
    if (!_user->add_mode("o"))
        return display_reply(ERR_NOOPERHOST);
    return display_reply(RPL_YOUREOPER);
}
void command::MODE(){
    if (_command.size() < 2)
        return display_reply(ERR_NEEDMOREPARAMS, "MODE");
    if (_command[1] != _user->_nick)
        return display_reply(ERR_USERSDONTMATCH, _user->_nick.c_str());
    if (_command[2][0] == '+')
        if (!_user->add_mode(_command[2]))
           return display_reply(ERR_UMODEUNKNOWNFLAG, _command[2].c_str());
    if (_command[2][0] == '-')
        if (!_user->deleteUserMode(_command[2][1]))
            return display_reply(ERR_UMODEUNKNOWNFLAG, _command[2].c_str());
}
void command::WHOIS(){
    if (_command.size() < 2)
        return display_reply(ERR_NONICKNAMEGIVEN);
    for (std::map<int, user *>::iterator it = _user->_serv->_users.begin(); it != _user->_serv->_users.end(); it++)
        if (it->second->_nick == _command[1])
                return display_reply(RPL_WHOISUSER, _user->_nick.c_str(), _user->_realname.c_str(), _user->_serv->_name.c_str(), _user->_realname.c_str());
    return display_reply(ERR_NOSUCHNICK, _command[1].c_str());
}
void command::PART(){
    if (_command.size() < 2)
        return display_reply(ERR_NEEDMOREPARAMS, _command[0].c_str());
    
}
void command::QUIT(){}

void command::PING(){
    std::string reply = "PONG : ";
    if (_command.size() < 2)
        return display_reply(ERR_NOORIGIN);
    reply += _user->_serv->_name.c_str();
    return display_reply(reply);
}
void command::PONG(){
    if (_command.size() < 2)
        return display_reply(ERR_NOORIGIN);
}

std::ostream &operator<<(std::ostream &o, command &rhs){
    for(std::vector<std::string>::iterator it = rhs._command.begin(); it != rhs._command.end(); it++){
        o << *it;
        if (it != rhs._command.end()-1)
            std::cout << " ";
    }
    return (o);
}