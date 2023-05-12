#include "command.hpp"

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
    std::cout << "Reply: " << message << std::endl;
}

void command::init_func_map(){
    _map_fonction.insert(std::make_pair("PASS",&command::PASS));
    _map_fonction.insert(std::make_pair("USER",&command::USER));
    _map_fonction.insert(std::make_pair("NICK",&command::NICK));
    _map_fonction.insert(std::make_pair("JOIN",&command::JOIN));
    _map_fonction.insert(std::make_pair("OPER",&command::OPER));
    _map_fonction.insert(std::make_pair("MODE",&command::MODE));
    _map_fonction.insert(std::make_pair("PART",&command::PART));
    _map_fonction.insert(std::make_pair("QUIT",&command::QUIT));
}

void command::PASS(){
    if (_command.size() < 2)
        display_reply(ERR_NEEDMOREPARAMS, "PASS");
    else if (_user->_mode == 1){
        display_reply(ERR_ALREADYREGISTRED);
    }
    else if (_command[1] == _user->_serv->_password && _user->_nick.size() > 0 && _user->_realname.size() > 0){
        _user->_mode = 1;
        display_reply(RPL_WELCOME, _user->_nick.c_str(), _user->_realname.c_str(), _user->_serv->_name.c_str());
    }
}
void command::USER(){
    if (_command.size() < 5){
        display_reply(ERR_NEEDMOREPARAMS, "USER");
        return ;    
    }
    _user->_realname = _command[4];
}
void command::NICK(){
    if (_command.size() < 2){
        display_reply(ERR_NONICKNAMEGIVEN);
        return ;
    }
    _user->_nick = _command[1];
}
void command::JOIN(){}
void command::OPER(){}
void command::MODE(){}
void command::PART(){}
void command::QUIT(){}

std::ostream &operator<<(std::ostream &o, command &rhs){
    for(std::vector<std::string>::iterator it = rhs._command.begin(); it != rhs._command.end(); it++){
        o << *it;
        if (it != rhs._command.end()-1)
            std::cout << " ";
    }
    return (o);
}