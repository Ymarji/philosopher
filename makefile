NAME = philo
LIB = philo.a
SRC = ./filo_main.c \
	# ./ls_utile.c


LIBFT = ./libft/libft.a

OBJ = $(SRC:.c=.o)

# FLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME) : $(OBJ) 
	@ar rcs $(LIB) $(OBJ)
	@cd ./libft && make
	gcc $(FLAGS) $(LIB) $(LIBFT) -o $(NAME)

%.o: %.c
	gcc $(FLAGS) -o $@ -c $<

clean:
	@cd ./libft && make -s fclean
	@rm -rf $(OBJ) $(LIB)

fclean: clean
	@rm -f $(NAME)

re: fclean all