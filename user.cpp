#include "user.hpp"

user::user(){
}

user::user(int fd, sockaddr_in address):_address(address), _fd(fd){
    (void)_fd;
    (void)_address;
}

user::~user(){
}