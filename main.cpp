#include "serveur.hpp"
#include <sstream>
int erroring(const char *message){
	std::cout << "Error: " << message << std::endl;
	return (0);
}

int main(int ac, char **av){
	if (ac != 3)
		return erroring("Number of arguments");

	std::stringstream sstream(av[1]);
	int i;
	sstream >> i;
	if (sstream.fail() || !sstream.eof() || i < 0)
	 	return erroring("Port incorrect");

	serveur serv(i,av[2]);
	serv.affichage();

	// int sock = socket(AF_INET, SOCK_STREAM, 0);
	// if (sock == -1)
	// 	return erroring ("Failed to create socket");

	// sockaddr_in adress;
	// adress.sin_family = AF_INET;
	// adress.sin_addr.s_addr = INADDR_ANY;
	// adress.sin_port = htons(serv.getport());

	// if (bind(sock, (struct sockaddr*)&adress, sizeof(adress)) < 0)
	// 	return erroring("Failed bind to port");
	
	// if (listen(sock, 10) < 0)
	// 	return erroring("Failed to listen on socket");

	// // Grab a connection from the queue
	// int addrlen = sizeof(adress);
	// int connection = accept(sock, (struct sockaddr*)&adress, (socklen_t*)&addrlen);
	// if (connection < 0) 
	// 	return erroring("Failed to grab connection");

	// // Close the connections
	// close(connection);
	// close(sock);
}