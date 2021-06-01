/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filo_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymarji <ymarji@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/01 13:27:09 by ymarji            #+#    #+#             */
/*   Updated: 2021/06/01 15:46:51 by ymarji           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Philosophers.h"

void init_philo(t_var *var)
{
	int i;

	i = 0;
	while (i < var->num_of_philo)
	{
		var->phil[i].index = i;
		var->phil[i].nbr = i + 1;
		var->phil[i].right = i;
		var->phil[i].left = (i + 1) % var->num_of_philo;
		var->phil[i].var = var;
		var->phil[i].nmbrofmeal = var->arg.num_eat;
		i++;
	}
}

int	cheack_args(t_var *var, char *av)
{
	if (var->num_of_philo < 0 || var->arg.time_to_die < 0 ||
	var->arg.time_to_eat < 0 || var->arg.time_to_sleep < 0)
	{
		ft_putendl_fd("Error", 2);
		return (1);
	}
	if (av)
	{
		if (var->arg.num_eat < 0)
		{
			ft_putendl_fd("Error", 2);
			return (1);
		}
	}
	return (0);
}

int get_args(t_var *var, char **av)
{
	int i;
	t_philo *new;

	i = 1;
	while (av[i])
	{
		if (!ft_isdigit_str(av[i]))
		{
			ft_putendl_fd("Error", 2);
			return (1);
		}
		i++;
	}
	var->num_of_philo = ft_atoi(av[1]);
	var->arg.time_to_die = ft_atoi(av[2]);
	var->arg.time_to_eat = ft_atoi(av[3]);
	var->arg.time_to_sleep = ft_atoi(av[4]);
	if (av[5])
		var->arg.num_eat = ft_atoi(av[5]);
	else
		var->arg.num_eat = -1;
	return (cheack_args(var, av[5]));
}

long get_time(int ind, long start)
{
	struct timeval time;
	long	count;

	gettimeofday(&time, NULL);
	count = time.tv_sec * 1000 + time.tv_usec / 1000;
	if (ind == 0)
		return (count);
	else
		return (count - start);
}

void sleeping(t_var *var, t_philo *phil)
{
	print_stat(var, "%lu %d is sleeping\n", get_time(1, var->start), phil->nbr);
	usleep(var->arg.time_to_sleep * 1000);
}

void eat(t_var *var, t_philo *phil)
{
	print_stat(var, "%lu %d is eating\n", get_time(1, var->start), phil->nbr);
	phil->timofdeath = get_time(0, 0) + var->arg.time_to_die;
	var->totalmeal++;
	usleep(var->arg.time_to_eat * 1000);
}

void *to_die(void *arg)
{
	t_philo *philo;
	t_var *var;

	philo = (t_philo *)arg;
	var = philo->var;
	while (1)
	{
		pthread_mutex_lock(&var->death_lock);
		if (get_time(0, 0) > philo->timofdeath)
		{
			pthread_mutex_lock(&var->print_lock);
			printf("\033[31m%lu %d died\033[0m\n", get_time(1, var->start), philo->nbr);
			pthread_mutex_unlock(&var->mt);
			break;
		}
		if (var->totalmeal == var->arg.num_eat * var->num_of_philo)
		{
			printf("%d\n", var->totalmeal);
			pthread_mutex_lock(&var->print_lock);
			printf("\033[32m %d DONE\033[0m\n", philo->index);
			pthread_mutex_unlock(&var->print_lock);
			pthread_mutex_unlock(&var->mt);
			break;
		}
		pthread_mutex_unlock(&var->death_lock);
		usleep(500);
	}
	return NULL;
}

void life_end(t_var *var, t_philo *phil)
{
	pthread_create(&var->tid, NULL, &to_die, phil);
	pthread_detach(var->tid);
}

void	print_stat(t_var *var, char *msg, long arg, int nbr)
{
	pthread_mutex_lock(&var->print_lock);
	printf(msg,  arg, nbr);
	pthread_mutex_unlock(&var->print_lock);
}

void *philosopher_life(void *arg)
{
	t_var *var;
	t_philo *t;
	int i = 0;

	t = (t_philo *)arg;
	var = t->var;
	t->timofdeath = get_time(0, 0) + var->arg.time_to_die;
	life_end(var, t);
	while (1)
	{
		pthread_mutex_lock(&var->fork[t->right]);
		print_stat(var, "%lu %d has taken a fork\n", get_time(1, var->start), t->nbr);
		pthread_mutex_lock(&var->fork[t->left]);
		print_stat(var, "%lu %d has taken a fork\n", get_time(1, var->start), t->nbr);
		eat(var, t);
		pthread_mutex_unlock(&var->fork[t->right]);
		pthread_mutex_unlock(&var->fork[t->left]);
		sleeping(var, t);
		print_stat(var, "%lu %d is thinking\n", get_time(1, var->start), t->nbr);
		if (--t->nmbrofmeal == 0)
			while (1)
			{
				t->timofdeath = get_time(0, 0) + var->arg.time_to_die;
				usleep(500);
			}
	}
	return (NULL);
}

void creat_thread(t_var *var)
{
	int i;

	var->fork = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * (var->num_of_philo));
	var->phil = (t_philo *)malloc(sizeof(t_philo) * (var->num_of_philo));
	init_philo(var);
	i = -1;
	while (++i < var->num_of_philo)
		pthread_mutex_init(&(var->fork[i]), NULL);
	i = 0;
	var->start = get_time(0, 0);
	while (i < var->num_of_philo)
	{

		pthread_create(&var->tid, NULL, &philosopher_life, &var->phil[i]);
		pthread_detach(var->tid);
		usleep(100);
		i++;
	}
}

void	ft_free(t_var *var, int step)
{
	int i;

	i = -1;
		
	if (var)
	{
		if (step == END)
		{
			while (++i < var->num_of_philo)
				pthread_mutex_destroy(&var->fork[i]);
			if (var->phil)
				free(var->phil);
		}
		free(var);
	}
}

int init(t_var *var)
{
	if (pthread_mutex_init(&var->mt, NULL) ||
	pthread_mutex_init(&var->print_lock, NULL) ||
	pthread_mutex_init(&var->death_lock, NULL))
	{
		ft_putendl_fd("Error: init mutex !!\n", 2);
		return (0);
	}
	var->phil = NULL;

	return (1);
}

int main(int ac, char **av)
{
	t_var *var;
	int	i = -1;

	var = (t_var *)malloc(sizeof(t_var));
	if (ac <= 4 || ac > 6 || !init(var))
	{
		ft_free(var, START);
		return (1);
	}
	if (!get_args(var, av))
	{
		pthread_mutex_lock(&var->mt);
		creat_thread(var);
		pthread_mutex_lock(&var->mt);
		pthread_mutex_destroy(&var->mt);
		pthread_mutex_destroy(&var->death_lock);
		pthread_mutex_destroy(&var->print_lock);
		ft_free(var, END);
	}
	return (0);
}
