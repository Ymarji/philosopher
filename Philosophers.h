
#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include "./libft/libft.h"
# include <sys/time.h>

# define START 0
# define END 1

typedef	struct	s_philo
{
	int		nbr;
	int		index;
	int		left;
	int		right;
	long	timofdeath;
	int		nmbrofmeal;
	struct	s_var	*var;
}				t_philo;

typedef struct	s_arg
{
	int	time_to_die;
	int	time_to_sleep;
	int	time_to_eat;
	int num_eat;
}				t_arg;

typedef struct	s_var
{
	int		totalmeal;
	pthread_mutex_t death_lock;
	pthread_mutex_t print_lock;
	long	start;
	long	end;
	pthread_mutex_t mt;
	pthread_mutex_t *fork;
	pthread_t	tid;
	int		num_of_philo;
	t_philo *phil;
	t_arg	arg;
}				t_var;

int		ft_lstsize_m(t_philo *lst);
t_philo	*ft_lstnew_m(int content);
t_philo	*ft_lstlast_m(t_philo *lst);
void	ft_lstadd_back_m(t_philo **alst, t_philo *new);

void	print_stat(t_var *var, char *msg, long arg, int nbr);
#endif