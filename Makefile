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
		command.hpp \
		replies.hpp

OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ) $(INC)
	@echo "\033[0;32m [OK] \033[0m       \033[0;33m Compiling:\033[0m" $(SRC)
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@echo "\033[0;32m [OK] \033[0m       \033[0;33m Creating:\033[0m" $(NAME)

.cpp.o:
	@$(CC) $(CFLAGS) -c $<

clean:
	@rm -f $(OBJ)
	@echo "\033[0;32m [OK] \033[0m       \033[0;31m Delete:\033[0m" $(OBJ)

fclean: clean
	@rm -f $(NAME)
	@echo "\033[0;32m [OK] \033[0m       \033[0;31m Delete:\033[0m" $(NAME)

po: all clean

re: fclean all

.PHONY: all re clean fclean

.SILENT: $(OBJS)