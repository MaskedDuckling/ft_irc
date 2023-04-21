#ifndef SERVEUR_HPP
# define SERVEUR_HPP

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <unistd.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <vector>

/*A faire*/

// Fonction d'assignation
// Remttre en prive

class serveur{
    private:
        std::vector<pollfd> _pollfds; 
        int			_port;
        std::string	_password;
        int         _main_socket;
        sockaddr_in _adress;

    public:

    /*          Constructeurs && Destructeurs           */

        serveur();
        serveur(int port, std::string password);
        serveur(const serveur & rhs);
        ~serveur();

    /*          Fonctions Membres           */

        int init();
        int loop();
        

    /*          Operateurs          */

        serveur &operator=(serveur const & rhs);

    /*          Utils           */

        int         getport();
        std::string	getpassword();
        int         getmain_socket();
        sockaddr_in getadress();

        pollfd      createfd(int fd);

        void        affichage();
        int         erroring(const char *message);
};

#endif