#include "./philo.h"

long int	time_elapsed(t_timeval *starting_time)
{
    long int    timer;
    t_timeval   curr_time;

    gettimeofday(&curr_time, NULL);
    timer = ((curr_time.tv_sec - starting_time->tv_sec) * 1000) + ((curr_time.tv_usec - starting_time->tv_usec) / 1000);
    return (timer);
}

int	main(void)
{
	t_timeval starting_time;
	long int timer;

	gettimeofday(&starting_time, NULL);
	usleep(1205020);
	timer = time_elapsed(&starting_time);
    printf("timer = %ld\n", timer);
}