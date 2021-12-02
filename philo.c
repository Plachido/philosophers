/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plpelleg <plpelleg@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 14:32:45 by plpelleg          #+#    #+#             */
/*   Updated: 2021/12/02 23:55:53 by plpelleg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	*ft_end_check(void *philosopher)
{
	int				i;
	t_philo			*philo;
	struct timeval	now;

	i = -1;
	philo = (t_philo *)philosopher;
	gettimeofday(&now, NULL);
	while (++i || 1)
	{
		if ((philo[i].last_time_eaten.tv_usec - now.tv_usec
			) > (long int)philo[i].parameters-> time_to_die)
		{
			ft_print(&philo[i], 5);
			break ;
		}
		else if (philo[i].parameters->n_times_to_eat > -1
			&& philo[i].num_times_eaten >= philo[i].parameters->n_times_to_eat)
		{
			ft_print(&philo[i], 6);
			break ;
		}
		if (i == philo[i].parameters->n_philo - 1)
			i = -1;
	}
	return (NULL);
}

static void	ft_start_eating(t_philo *philosopher)
{
	pthread_mutex_lock(philosopher->left_fork);
	ft_print(philosopher, 1);
	pthread_mutex_unlock(philosopher->left_fork);
	pthread_mutex_lock(philosopher->right_fork);
	ft_print(philosopher, 1);
	pthread_mutex_unlock(philosopher->right_fork);
	ft_print(philosopher, 2);
	philosopher-> num_times_eaten += 1;
}

static void	ft_drop_forks(t_philo *philosopher)
{
	pthread_mutex_unlock(philosopher->left_fork);
	pthread_mutex_unlock(philosopher->right_fork);
}

void	*ft_routine(void *philo)
{
	t_philo	*philosopher;

	philosopher = (t_philo *)philo;
	gettimeofday(&philosopher->last_time_eaten, NULL);
	while (1)
	{
		ft_print(philosopher, 4);
		ft_start_eating(philosopher);
		usleep(philosopher->parameters->time_to_eat);
		ft_drop_forks(philosopher);
		gettimeofday(&philosopher->last_time_eaten, NULL);
		ft_print(philosopher, 3);
		usleep(philosopher->parameters->time_to_sleep);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	pthread_mutex_t	*forks;
	pthread_t		*threads;
	t_param			*parameters;
	t_philo			*philosophers;

	parameters = ft_parse(argc, argv);
	if (!parameters)
		return (0);
	pthread_mutex_init(&parameters->print, NULL);
	forks = malloc(sizeof(pthread_mutex_t) * parameters->n_philo + 1);
	threads = malloc(sizeof(pthread_t) * (parameters ->n_philo + 1));
	philosophers = malloc(sizeof(t_philo) * parameters->n_philo);
	if (!forks || !threads || !philosophers)
		return (ft_exit(parameters, NULL, NULL, NULL));
	ft_initialize_mutex(parameters, forks);
	ft_init_philo(forks, parameters, philosophers);
	pthread_create(&threads[parameters ->n_philo], NULL,
		&ft_end_check, philosophers);
	ft_generate_threads(parameters, threads, philosophers);
	pthread_join(threads[parameters->n_philo], NULL);
	ft_exit(parameters, forks, threads, philosophers);
	return (0);
}
