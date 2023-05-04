/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsimon <nsimon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 14:48:44 by nsimon            #+#    #+#             */
/*   Updated: 2023/05/04 15:59:01 by nsimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PING_H
# define FT_PING_H

# include "libft.h"

# include <signal.h>
# include <netinet/ip_icmp.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <sys/time.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <errno.h>

# define	SOCK_TYPE SOCK_RAW
// # define	SOCK_TYPE SOCK_DGRAM

typedef struct
{
	struct iphdr	ip;
	struct icmphdr	icmp;
	char			data[1];
}	t_recv;

struct s_time
{
	double 			value;
	struct s_time 	*next;
};

struct s_data
{
	uint16_t		id;
	struct timeval	time;
	struct s_data	*next;
};

struct		s_ping
{
	int				sock;
	char			*host;
	char			*ip;
	uint16_t 		id;
	int				sent;
	int				recv;
	unsigned int	interval;

	struct s_data	*pingData;
	struct s_time	*timeStats;
	struct addrinfo	*res_addrinfo;
};

double			min(struct s_time *timeStats);
double			max(struct s_time *timeStats);
double			avg(struct s_time *timeStats);
double			stddev(struct s_time *timeStats);
void			add_time(struct s_time **timeStats, double value);
void			clear_time(struct s_time **timeStats);
void 			add_data(struct s_data **list, int id, struct timeval time);
void			remove_data(struct s_data **list, int id);
struct timeval *get_data(struct s_data *list, int id);
void 			clear_data(struct s_data **list);

#endif