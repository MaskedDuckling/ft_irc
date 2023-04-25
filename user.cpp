#include "user.hpp"

user::user(){
}

user::user(int fd): _fd(fd){
    (void)_fd;
}

user::~user(){
}