/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsimon <nsimon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 14:48:44 by nsimon            #+#    #+#             */
/*   Updated: 2023/05/03 15:55:30 by nsimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PING_H
# define FT_PING_H

# include "libft.h"

# include <signal.h>
# include <netinet/ip.h>
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

typedef struct		s_recv
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

struct		s_ping
{
	int				sock;
	char			*host;
	char			*ip;
	uint16_t 		id;
	int				sent;
	int				recv;
	int				lost;
	unsigned int	interval;

	struct s_time	*timeData;
	struct addrinfo	*res_addrinfo;
};

double		min(struct s_time *timeData);
double		max(struct s_time *timeData);
double		avg(struct s_time *timeData);
double		stddev(struct s_time *timeData);
void		add_time(struct s_time **timeData, double value);
void		clear_time(struct s_time **timeData);

#endif