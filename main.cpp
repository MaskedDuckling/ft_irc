#include "serveur.hpp"

int main(int ac, char **av){
	if (ac != 3)
		return 0;

	std::stringstream sstream(av[1]);
	int i;
	sstream >> i;
	if (sstream.fail() || !sstream.eof() || i < 0)
	 	return 0;

	serveur serv(i,av[2]);
	serv.affichage();
	std::cout << "Exit value : [" << serv.init()  << "]" << std::endl;
}