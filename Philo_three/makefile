NAME = philo
LIB = philo.a
SRC = ./philo_main.c \
	./philo_utils.c \
	./philo_arg.c \
	./philo_prosses.c



OBJ = $(SRC:.c=.o)

FLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME) : $(OBJ) 
	@ar rcs $(LIB) $(OBJ)
	gcc $(FLAGS) $(LIB) -o $(NAME)

%.o: %.c
	gcc $(FLAGS) -o $@ -c $<

clean:
	@rm -rf $(OBJ) $(LIB)

fclean: clean
	@rm -f $(NAME)

re: fclean all