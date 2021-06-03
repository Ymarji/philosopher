
#ifndef PHILOSOPHERS_H
#define PHILOSOPHERS_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <sys/time.h>


#define START 0
#define END 1

typedef struct s_philo
{
	int nbr;
	int index;
	int left;
	int right;
	long timofdeath;
	int nmbrofmeal;
	struct s_var *var;
} t_philo;

typedef struct s_arg
{
	int time_to_die;
	int time_to_sleep;
	int time_to_eat;
	int num_eat;
} t_arg;

typedef struct s_var
{
	pid_t *pid;
	int totalmeal;
	sem_t *death_lock;
	sem_t *print_lock;
	long start;
	long end;
	sem_t *mt;
	sem_t *fork;
	pthread_t tid;
	sem_t *meal;
	int n_ph;
	t_philo *phil;
	t_arg arg;
} t_var;

int ft_lstsize_m(t_philo *lst);
t_philo *ft_lstnew_m(int content);
t_philo *ft_lstlast_m(t_philo *lst);
void ft_lstadd_back_m(t_philo **alst, t_philo *new);

void p_msg(t_var *var, char *msg, long arg, int nbr);




void	ft_putstr_fd(char *s, int fd);
void	ft_putchar_fd(char c, int fd);
void	ft_putendl_fd(char *s, int fd);
int		ft_isdigit_str(char *str);
int		ft_isdigit(int c);
int		ft_atoi(const char *str);
#endif