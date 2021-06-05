/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymarji <ymarji@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/05 12:11:10 by ymarji            #+#    #+#             */
/*   Updated: 2021/06/05 12:11:12 by ymarji           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <semaphore.h>
# include <signal.h>
# include <sys/time.h>

# define START 0
# define END 1

typedef struct s_philo
{
	int				nbr;
	int				index;
	int				left;
	int				right;
	long			timofdeath;
	int				nmbrofmeal;
	struct s_var	*var;
}			t_philo;

typedef struct s_arg
{
	int		time_to_die;
	int		time_to_sleep;
	int		time_to_eat;
	int		num_eat;
}			t_arg;

typedef struct s_var
{
	pid_t		*pid;
	int			totalmeal;
	sem_t		*death_lock;
	sem_t		*print_lock;
	long		start;
	long		end;
	sem_t		*mt;
	sem_t		*fork;
	pthread_t	tid;
	sem_t		*meal;
	int			n_ph;
	t_philo		*phil;
	t_arg		arg;
}			t_var;

int		ft_lstsize_m(t_philo *lst);
t_philo	*ft_lstnew_m(int content);
t_philo	*ft_lstlast_m(t_philo *lst);
void	ft_lstadd_back_m(t_philo **alst, t_philo *new);

void	creat_pross(t_var *var);
void	*to_die(void *arg);
void	*philosopher_life(void *arg);
void	life_end(t_var *var, t_philo *phil);

void	*meal_watch(void *arg);
int		get_args(t_var *var, char **av);
int		cheack_args(t_var *var, char *av);
void	init_philo(t_var *var);
long	get_time(int ind, long start);
void	ft_free(t_var *var, int step);

void	p_msg(t_var *var, char *msg, long arg, int nbr);
void	eat(t_var *var, t_philo *phil);
void	sleeping(t_var *var, t_philo *phil);

void	ft_putchar_fd(char c, int fd);
void	ft_putendl_fd(char *s, int fd);
int		ft_isdigit_str(char *str);
int		ft_isdigit(int c);
int		ft_atoi(const char *str);
#endif