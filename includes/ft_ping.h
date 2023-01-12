/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsimon <nsimon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 14:48:44 by nsimon            #+#    #+#             */
/*   Updated: 2023/01/12 14:50:29 by nsimon           ###   ########.fr       */
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

struct	s_global
{
	int		sock;
	char	*host;
	int		sent;
	int		recv;
	int		time;
}		g_ping;

#endif