/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filo_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymarji <ymarji@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/01 13:27:09 by ymarji            #+#    #+#             */
/*   Updated: 2021/06/01 16:31:53 by ymarji           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Philosophers.h"

void	init_philo(t_var *var)
{
	int	i;

	i = 0;
	while (i < var->n_ph)
	{
		var->phil[i].index = i;
		var->phil[i].nbr = i + 1;
		var->phil[i].right = i;
		var->phil[i].left = (i + 1) % var->n_ph;
		var->phil[i].var = var;
		var->phil[i].nmbrofmeal = var->arg.num_eat;
		i++;
	}
}

int	cheack_args(t_var *var, char *av)
{
	if (var->n_ph < 0 || var->arg.time_to_die < 0
		|| var->arg.time_to_eat < 0 || var->arg.time_to_sleep < 0)
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

int	get_args(t_var *var, char **av)
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
	var->n_ph = ft_atoi(av[1]);
	var->arg.time_to_die = ft_atoi(av[2]);
	var->arg.time_to_eat = ft_atoi(av[3]);
	var->arg.time_to_sleep = ft_atoi(av[4]);
	if (av[5])
		var->arg.num_eat = ft_atoi(av[5]);
	else
		var->arg.num_eat = -1;
	return (cheack_args(var, av[5]));
}

long	get_time(int ind, long start)
{
	struct timeval	time;
	long			count;

	gettimeofday(&time, NULL);
	count = time.tv_sec * 1000 + time.tv_usec / 1000;
	if (ind == 0)
		return (count);
	else
		return (count - start);
}

void	sleeping(t_var *var, t_philo *phil)
{
	p_msg(var, "%lu %d is sleeping\n", get_time(1, var->start), phil->nbr);
	usleep(var->arg.time_to_sleep * 1000);
}

void	eat(t_var *var, t_philo *phil)
{
	p_msg(var, "%lu %d is eating\n", get_time(1, var->start), phil->nbr);
	phil->timofdeath = get_time(0, 0) + var->arg.time_to_die;
	var->totalmeal++;
	usleep(var->arg.time_to_eat * 1000);
}

void	*to_die(void *arg)
{
	t_philo	*ph;
	t_var	*var;

	ph = (t_philo *)arg;
	var = ph->var;
	while (1)
	{
		pthread_mutex_lock(&var->death_lock);
		if (get_time(0, 0) > ph->timofdeath)
		{
			pthread_mutex_lock(&var->print_lock);
			printf("%lu %d died\n", get_time(1, var->start), ph->nbr);
			pthread_mutex_unlock(&var->mt);
			break;
		}
		if (var->totalmeal == var->arg.num_eat * var->n_ph)
		{
			printf("%d\n", var->totalmeal);
			pthread_mutex_lock(&var->print_lock);
			printf("SIMULATION DONE");
			pthread_mutex_unlock(&var->print_lock);
			pthread_mutex_unlock(&var->mt);
			break;
		}
		pthread_mutex_unlock(&var->death_lock);
		usleep(500);
	}
	return NULL;
}

void	life_end(t_var *var, t_philo *phil)
{
	pthread_create(&var->tid, NULL, &to_die, phil);
	pthread_detach(var->tid);
}

void	p_msg(t_var *var, char *msg, long arg, int nbr)
{
	pthread_mutex_lock(&var->print_lock);
	printf(msg, arg, nbr);
	pthread_mutex_unlock(&var->print_lock);
}

void	*philosopher_life(void *arg)
{
	t_var	*var;
	t_philo	*ph;
	int		i;

	i = 0;
	ph = (t_philo *)arg;
	var = ph->var;
	ph->timofdeath = get_time(0, 0) + var->arg.time_to_die;
	life_end(var, ph);
	while (1)
	{
		pthread_mutex_lock(&var->fork[ph->right]);
		p_msg(var, "%lu %d has taken a fork\n",
		get_time(1, var->start), ph->nbr);
		pthread_mutex_lock(&var->fork[ph->left]);
		p_msg(var, "%lu %d has taken a fork\n",
		get_time(1, var->start), ph->nbr);
		eat(var, ph);
		pthread_mutex_unlock(&var->fork[ph->right]);
		pthread_mutex_unlock(&var->fork[ph->left]);
		sleeping(var, ph);
		p_msg(var, "%lu %d is thinking\n", get_time(1, var->start), ph->nbr);
	}
	return (NULL);
}

void	creat_thread(t_var *var)
{
	int	i;

	var->fork = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * (var->n_ph));
	var->phil = (t_philo *)malloc(sizeof(t_philo) * (var->n_ph));
	init_philo(var);
	i = -1;
	while (++i < var->n_ph)
		pthread_mutex_init(&(var->fork[i]), NULL);
	i = 0;
	var->start = get_time(0, 0);
	while (i < var->n_ph)
	{

		pthread_create(&var->tid, NULL, &philosopher_life, &var->phil[i]);
		pthread_detach(var->tid);
		usleep(100);
		i++;
	}
}

void	ft_free(t_var *var, int step)
{
	int	i;

	i = -1;

	if (var)
	{
		if (step == END)
		{
			while (++i < var->n_ph)
				pthread_mutex_destroy(&var->fork[i]);
			if (var->phil)
				free(var->phil);
		}
		free(var);
	}
}

int	init(t_var *var)
{
	if (pthread_mutex_init(&var->mt, NULL)
	|| pthread_mutex_init(&var->print_lock, NULL)
	|| pthread_mutex_init(&var->death_lock, NULL))
	{
		ft_putendl_fd("Error: init mutex !!\n", 2);
		return (0);
	}
	var->phil = NULL;
	return (1);
}

int	main(int ac, char **av)
{
	t_var	*var;
	int	i;
	
	i = -1;
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
