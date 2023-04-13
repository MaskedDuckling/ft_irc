#include "serveur.hpp"

int erroring(const char *message){
	std::cout << "Error: " << message << std::endl;
	return (0);
}

int main(int ac, char **av){
	if (ac != 3)
		return erroring("Number of arguments");
	if (av[1][0] != '0' && std::atoi(av[1]) == 0)
		return erroring("Port incorrect");

	serveur serv(std::atoi(av[1]),av[2]);
	serv.affichage();

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		return erroring ("Failed to create socket");

	sockaddr_in adress;
	adress.sin_family = AF_INET;
	adress.sin_addr.s_addr = INADDR_ANY;
	adress.sin_port = htons(serv.getport());

	if (bind(sock, (struct sockaddr*)&adress, sizeof(adress)) < 0)
		return erroring("Failed bind to port");
	
	if (listen(sock, 10) < 0)
		return erroring("Failed to listen on socket");
}