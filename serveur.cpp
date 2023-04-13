#include "serveur.hpp"

/*          Constructeurs && Destructeurs           */

serveur::serveur(): _port(0), _password(NULL){}
serveur::serveur(int port, std::string password): _port(port), _password(password){}
serveur::serveur(const serveur & rhs){
    *this = rhs;
}
serveur::~serveur(){}

/*          Fonctions Membres           */

int serveur::getport(){
    return _port;
}

/*          Operateurs          */

serveur &serveur::operator=(serveur const & rhs){
    this->_password = rhs._password;
    this->_port = rhs._port;
    return (*this);
}

/*          Utils           */

void    serveur::affichage(){
    std::cout << "My_serv" << std::endl;
    std::cout << "Port : "<< this->_port << " \nPass : " << this->_password << std::endl;
}