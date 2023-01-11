/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsimon <nsimon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 14:48:08 by nsimon            #+#    #+#             */
/*   Updated: 2023/01/11 17:52:36 by nsimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ping.h"
#include <errno.h>

void	display_help(void)
{
	printf("\
Usage\n\
	ping [options] <destination>\n\
\n\
Options:\n\
	<destination>	dns name or ip address\n\
	-h		print help and exit\n\
	-v		verbose output\n\
");
}

void	sigint_handler(int signum)
{
	(void)signum;
	printf("\n--- %s ping statistics ---\n", g_ping.host);
	printf("%d packets transmitted, %d received, %d%% packet loss, time %dms",
		g_ping.sent, g_ping.recv,
		(g_ping.sent - g_ping.recv) * 100 / g_ping.sent, g_ping.time);
	exit(0);
}

void	start_ping(char verbose)
{
	struct addrinfo	*res;

	if (getaddrinfo(g_ping.host, NULL, NULL, &res) != 0)
	{
		printf("getaddrinfo: Name or service not known\n");
		exit(1);
	}
	g_ping.sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (g_ping.sock < 0)
	{
		printf("socket: %s\n", strerror(errno));
		exit(1);
	}
	printf("PING %s (%s) 56(84) bytes of data.\n", g_ping.host,
		inet_ntoa(((struct sockaddr_in *)res->ai_addr)->sin_addr));
	if (verbose)
	{
		printf("ft_ping: sock4.fd: %d (socktype: SOCK_DGRAM)", g_ping.sock);
		printf(", sock6.fd: -1 (socktype:0), hints.ai_family: AF_INET\n\n");
		printf("ai->ai_family: AF_INET, ai->ai_cannonname: '%s'\n",
			g_ping.host);
	}
	signal(SIGINT, sigint_handler);
	// ping(res, verbose);
	freeaddrinfo(res);
	close(g_ping.sock);
}

int	main(int argc, char **argv)
{
	char	verbose;

	verbose = 0;
	if (argc < 2)
	{
		printf("ft_ping: usage error: Destination address required\n");
		return (1);
	}
	if (!ft_strcmp(argv[1], "-h"))
	{
		display_help();
		exit(0);
	}
	if (!ft_strcmp(argv[1], "-v"))
		verbose = 1;
	if (verbose && argc < 3)
	{
		printf("ft_ping: usage error: Destination address required\n");
		return (1);
	}
	g_ping.host = argv[argc - 1];
	start_ping(verbose);
	return (0);
}
