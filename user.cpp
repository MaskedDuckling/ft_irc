#include "user.hpp"

user::user(){
}

user::user(int fd, sockaddr_in address):_address(address), _fd(fd){
    (void)_fd;
    (void)_address;
}

user::~user(){
}

void user::parse_commands(std::string message){
    std::vector<std::string> commands;
    while (message.find("\n") != std::string::npos)
    {
        commands.push_back(message.substr(0, message.find("\n")));
        message = message.substr(message.find("\n")+1);
    }
    for (std::vector<std::string>::iterator it = commands.begin() ; it != commands.end() ; it++)
    {
        parse_command(*it);
    }
}

void user::parse_command(std::string command){
    std::vector<std::string> split_command;
    while (command.find(" ") != std::string::npos)
    {
        split_command.push_back(command.substr(0, command.find(" ")));
        command = command.substr(command.find(" ")+1);
    }
    if (command.size() > 0)
        split_command.push_back(command);
    for (std::vector<std::string>::iterator it = split_command.begin() ; it != split_command.end() ; it++)
    {
        std::cout << *it << std::endl;
    }
    
    execute(split_command);
}

void user::execute(std::vector<std::string> split_command){
    if (split_command[0] == "NICK"){
        this->_nick = split_command[1];
    }
    if (split_command[0] == "USER"){
        this->_realname = split_command[1];
    }
    if (split_command[0] == "QUIT"){
        exit(0);
    }
    if (split_command[0] == "PRINT"){
        std::cout << "User::nick = " << this->_nick << "  User::realname = " << this->_realname << std::endl;
    }
}