#include "user.hpp"

user::user(){
}

user::user(int fd, sockaddr_in address):_address(address), _fd(fd){
    (void)_fd;
    (void)_address;
}

user::~user(){
}

void user::parse(std::string message){
    std::cout << message << std::endl;
}