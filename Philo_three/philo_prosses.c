/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_prosses.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymarji <ymarji@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/05 11:53:25 by ymarji            #+#    #+#             */
/*   Updated: 2021/06/05 12:07:26 by ymarji           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Philosophers.h"

void	creat_pross(t_var *var)
{
	int	i;

	var->pid = (pid_t *)malloc(sizeof(pid_t) * var->n_ph);
	sem_unlink("/fork");
	sem_unlink("/meal");
	var->fork = sem_open("/fork", O_CREAT, 777, var->n_ph);
	var->meal = sem_open("/meal", O_CREAT, 777, 0);
	var->phil = (t_philo *)malloc(sizeof(t_philo) * (var->n_ph));
	init_philo(var);
	i = 0;
	pthread_create(&var->tid, NULL, &meal_watch, var);
	pthread_detach(var->tid);
	var->start = get_time(0, 0);
	while (i < var->n_ph)
	{
		var->pid[i] = fork();
		if (var->pid[i] == 0)
		{
			philosopher_life(&var->phil[i]);
			exit(0);
		}
		i++;
	}
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
		sem_wait(var->fork);
		p_msg(var, "%lu %d has taken a fork\n",
			get_time(1, var->start), ph->nbr);
		sem_wait(var->fork);
		p_msg(var, "%lu %d has taken a fork\n",
			get_time(1, var->start), ph->nbr);
		eat(var, ph);
		sem_post(var->fork);
		sem_post(var->fork);
		sleeping(var, ph);
		p_msg(var, "%lu %d is thinking\n", get_time(1, var->start), ph->nbr);
	}
	return (NULL);
}

void	*meal_watch(void *arg)
{
	t_var	*var;
	int		nb;

	nb = 0;
	var = (t_var *)arg;
	while (1)
	{
		sem_wait(var->meal);
		nb++;
		if (nb == var->n_ph)
		{
			sem_wait(var->print_lock);
			printf("SIMULATION DONE\n");
			sem_post(var->mt);
			break ;
		}
	}
	return (NULL);
}

void	life_end(t_var *var, t_philo *phil)
{
	pthread_create(&var->tid, NULL, &to_die, phil);
	pthread_detach(var->tid);
}

void	*to_die(void *arg)
{
	t_philo	*ph;
	t_var	*var;

	ph = (t_philo *)arg;
	var = ph->var;
	while (1)
	{
		sem_wait(var->death_lock);
		if (get_time(0, 0) > ph->timofdeath)
		{
			sem_wait(var->print_lock);
			printf("%lu %d died\n", get_time(1, var->start), ph->nbr);
			sem_post(var->mt);
			break ;
		}
		sem_post(var->death_lock);
		usleep(500);
	}
	return (NULL);
}
