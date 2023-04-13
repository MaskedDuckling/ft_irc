#ifndef SERVEUR_HPP
# define SERVEUR_HPP

#include <iostream>
#include <cstdlib>
#include <unistd.h> 
#include <sys/socket.h>
#include <netinet/in.h>

/*A faire*/

// Fonction d'assignation
// Remttre en prive

class serveur{
    private:
        int			_port;
        std::string	_password;

    public:
        serveur();
        serveur(int port, std::string password);
        serveur(const serveur & rhs);
        ~serveur();

        int getport();

        serveur &operator=(serveur const & rhs);

        void affichage();
};

#endif