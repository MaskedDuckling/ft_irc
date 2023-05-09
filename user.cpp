#include "user.hpp"

user::user(){
}

user::user(int fd, sockaddr_in address, serveur *serv):_address(address), _fd(fd), _serv(serv), _mode(0){
    (void)_fd;
    (void)_address;
}

user::~user(){
}

void user::parse_commands(std::string message){
    while (message.find("\n") != std::string::npos)
    {
        command(message.substr(0, message.find("\n")), this);
        message = message.substr(message.find("\n")+1);
    }
    // for (std::vector<command>::iterator it = commands.begin() ; it != commands.end() ; it++)
    // {
    //     it->parse_command();
    // }
}