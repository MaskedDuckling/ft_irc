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
}

/*          Fonctions Membres           */

int serveur::init(){
    _main_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_main_socket == -1)
		return erroring ("Failed to create socket");
	sockaddr_in _adress;
	_adress.sin_family = AF_INET;
	_adress.sin_addr.s_addr = INADDR_ANY;
	_adress.sin_port = htons(_port);

	if (bind(_main_socket, (struct sockaddr*)&_adress, sizeof(_adress)) < 0)
		return erroring("Failed bind to port");
	
	if (listen(_main_socket, 10) < 0)
		return erroring("Failed to listen on socket");
    return loop();
}

int serveur::loop(){
    int addrlen = sizeof(_adress);
	_pollfds.push_back(createfd(accept(_main_socket, (struct sockaddr*)&_adress, (socklen_t*)&addrlen)));
    int ret = poll(_pollfds[0], 2, 500);
    while (ret > 0)
	{
        char buffer[100];
        int bytesRead = read(connection, buffer, 100);
        (void)bytesRead;
        std::cout << "The message was: " << buffer;

        std::string response = "Good talking to you\n";
        send(connection, response.c_str(), response.size(), 0);
	    close(connection);
    }
    if (connection < 0) {
        return erroring("Failed to grab connection");
    }

	// Close the connections
	close(_main_socket);
	shutdown(_main_socket, SHUT_RDWR);
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