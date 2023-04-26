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

    //Init _address structure
	sockaddr_in _address;
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = INADDR_ANY;
	_address.sin_port = htons(_port);
    memset(&(_address.sin_zero), '\0', 8);

    //Connect socket to _address and _port
	if (bind(_main_socket, (struct sockaddr*)&_address, sizeof(_address)) < 0)
		return erroring("Failed bind to port");

    //Wait for connection
	if (listen(_main_socket, 10) < 0)
		return erroring("Failed to listen on socket");

    //Put main socket in _pollfds
    _pollfds.push_back(createfd(_main_socket));
    return 0;
}

void    serveur::addUser(){
    struct sockaddr_in 	new_address;
	socklen_t			sin_size;
	sin_size = sizeof(struct sockaddr_in);	
	
    int new_fd = accept(_pollfds[0].fd, (struct sockaddr *)&new_address, &sin_size);
	if (new_fd == -1)
		return;
	
	/* Creation du user */
	_users[new_fd] = new user(new_fd, new_address);

	/* Maj de _pollfds */
	_pollfds.push_back(pollfd());
	_pollfds.back().fd = new_fd;
	_pollfds.back().events = POLLIN;
}

int serveur::loop(){
    char buff[1024];
    int lu = 0;
    std::string message;

    poll(&_pollfds[0], _pollfds.size(), 500);
    if (_pollfds[0].revents == POLLIN)
        addUser();
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
sockaddr_in serveur::getaddress(){
    return _address;
}

void    serveur::affichage(){
    std::cout << "My_serv" << std::endl;
    std::cout << "Port : "<< this->_port << " \nPass : " << this->_password << std::endl;
}

int serveur::erroring(const char *message){
	std::cout << "Error: " << message << std::endl;
	return (0);
}