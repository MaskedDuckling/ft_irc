NAME = ircserv

CC = c++

CFLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address

SRC =	main.cpp \
		serveur.cpp \
		user.cpp \
		channel.cpp \
		command.cpp

INC = serveur.hpp \
		user.hpp \
		channel.hpp \
		command.hpp

OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ) $(INC)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

.cpp.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

po: all clean

re: fclean all

.PHONY: all re clean fclean