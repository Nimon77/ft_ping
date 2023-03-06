/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsimon <nsimon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 14:48:44 by nsimon            #+#    #+#             */
/*   Updated: 2023/03/06 15:06:18 by nsimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PING_H
# define FT_PING_H

# include "libft.h"

# include <signal.h>
# include <netdb.h>
# include <netinet/ip_icmp.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <sys/time.h>
# include <errno.h>

typedef struct	s_ping_info
{
	struct timeval		tv_send;
	struct timeval		tv_recv;
	int					seq;
	struct s_ping_info	*next;
}		g_ping_info;

struct	s_ping
{
	int				sock;
	char			*host;
	char			*ip;
	int				sent;
	int				recv;
	int				lost;
	int				ttl;
	g_ping_info		*infos;
}		g_ping;

#endif