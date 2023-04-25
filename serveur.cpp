#include "serveur.hpp"

/*          Constructeurs && Destructeurs           */

serveur::serveur(): _port(0), _password(NULL){
}

serveur::serveur(int port, std::string password): _port(port), _password(password){
}

serveur::serveur(const serveur & rhs){
    *this = rhs;
}

serveur::~serveur(){
    close(_main_socket);
    shutdown(_main_socket, SHUT_RDWR);
}

/*          Fonctions Membres           */

int serveur::init(){
    //Create main socket fd
    _main_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_main_socket == -1)
		return erroring ("Failed to create socket");

    //Set socket as reusable and non-blocking
    int on = 1;
    if (setsockopt(_main_socket, SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on)) < 0)
        erroring("Failed setsockopt()");
    if (fcntl(_main_socket, F_SETFL, O_NONBLOCK) < 0)
		erroring("Failed fcntl()");

    //Init _adress structure
	sockaddr_in _adress;
	_adress.sin_family = AF_INET;
	_adress.sin_addr.s_addr = INADDR_ANY;
	_adress.sin_port = htons(_port);
    memset(&(_adress.sin_zero), '\0', 8);

    //Connect socket to _adress and _port
	if (bind(_main_socket, (struct sockaddr*)&_adress, sizeof(_adress)) < 0)
		return erroring("Failed bind to port");

    //Wait for connection
	if (listen(_main_socket, 10) < 0)
		return erroring("Failed to listen on socket");

    //Put main socket in _pollfds
    _pollfds.push_back(createfd(_main_socket));
    return 0;
}

int serveur::loop(){
    int addrlen = sizeof(_adress);
    char buff[1024];
    int lu = 0;
    std::string message;

    poll(&_pollfds[0], _pollfds.size(), 500);
    if (_pollfds[0].revents == POLLIN){
	    _pollfds.push_back(createfd(accept(_main_socket, (struct sockaddr*)&_adress, (socklen_t*)&addrlen)));
        _users.push_back(new user(_pollfds.back().fd));
    }
    else
    {
        for (std::vector<pollfd>::iterator it = _pollfds.begin()+1 ; it != _pollfds.end() ; it++)
        {
            if (it->revents == POLLIN){
                lu = read(it->fd, buff, 1024);
                std::cout << lu << std::endl;
                buff[lu] = '\0';
                message = std::string(buff);

                std::cout << "The message was: " << message;
            }
        }
    }
    return 1;
}

/*          Operateurs          */

serveur &serveur::operator=(serveur const & rhs){
    this->_password = rhs._password;
    this->_port = rhs._port;
    return (*this);
}

/*          Utils           */

pollfd serveur::createfd(int fd){
    pollfd ret;
    ret.fd = fd;
    ret.events = POLLIN;
    ret.revents = POLLOUT;
    return (ret);
}

int serveur::getport(){
    return _port;
}
std::string	serveur::getpassword(){
    return _password;
}
int         serveur::getmain_socket(){
    return _main_socket;
}
sockaddr_in serveur::getadress(){
    return _adress;
}

void    serveur::affichage(){
    std::cout << "My_serv" << std::endl;
    std::cout << "Port : "<< this->_port << " \nPass : " << this->_password << std::endl;
}

int serveur::erroring(const char *message){
	std::cout << "Error: " << message << std::endl;
	return (0);
}