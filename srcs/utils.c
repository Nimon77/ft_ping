#include "ft_ping.h"

double pow(double x, double y) {
	if (x == 0) {
		return 0.0;
	}
	if (y == 0) {
		return 1.0;
	}
	double result = 1.0;
	for (int i = 0; i < y; ++i) {
		result *= x;
	}
	return result;
}

double sqrt(double x) {
	double result = 1.0;
	for (int i = 0; i < 100; ++i) {
		result = (result + x / result) / 2;
	}
	return result;
}

double min(struct s_time *data) {
	if (!data)
		return 0.0;
	struct s_time *tmp = data;
	double min = data->value;
	while (tmp->next) {
		if (tmp->value < min) {
			min = tmp->value;
		}
		tmp = tmp->next;
	}
	return min;
}

double max(struct s_time *data) {
	if (!data)
		return 0.0;
	struct s_time *tmp = data;
	double max = data->value;
	while (tmp->next) {
		if (tmp->value > max) {
			max = tmp->value;
		}
		tmp = tmp->next;
	}
	return max;
}

double avg(struct s_time *data) {
	if (!data)
		return 0.0;
	double sum = 0.0;
	struct s_time *tmp = data;
	int size = 0;
	while (tmp->next) {
		++size;
		sum += tmp->value;
		tmp = tmp->next;
	}
	return sum / size;
}

double stddev(struct s_time *data) {
	if (!data)
		return 0.0;

	double mean, SD = 0.0;
	mean = avg(data);
	struct s_time *tmp = data;
	int size = 0;
	while (tmp->next) {
		++size;
		SD += pow(tmp->value - mean, 2);
		tmp = tmp->next;
	}
	return sqrt(SD / size);
}

void add_time(struct s_time **list, double value)
{
	struct s_time *new = malloc(sizeof(struct s_time));
	new->value = value;
	new->next = NULL;
	if (!*list) {
		*list = new;
		return ;
	}
	struct s_time *tmp = *list;
	while (tmp->next) {
		tmp = tmp->next;
	}
	tmp->next = new;
}

void clear_time(struct s_time **list)
{
	if (!*list)
		return;
	struct s_time *tmp = *list;
	while (tmp) {
		struct s_time *next = tmp->next;
		free(tmp);
		tmp = next;
	}
	*list = NULL;
}