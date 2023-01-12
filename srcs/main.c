/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsimon <nsimon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 14:48:08 by nsimon            #+#    #+#             */
/*   Updated: 2023/01/12 17:48:12 by nsimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ping.h"

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
	close(g_ping.sock);
	exit(0);
}

unsigned short checksum(unsigned short *addr, size_t size)
{
	register long sum = 0;
	for (; size > 1; size -= 2)
		sum += *addr++;
	if (size > 0)
		sum += *(unsigned char*)addr;
	while (sum >> 16)
		sum = (sum & 0xffff) + (sum >> 16);
	return (~sum);
}

int ping_loop(struct addrinfo *addr)
{
	struct icmphdr icmp = {0};

	icmp.type = ICMP_ECHO;
	icmp.un.echo.id = 0;
	icmp.un.echo.sequence = 0;
	while (1)
	{
		// icmp.un.echo.id += 1;
		// icmp.un.echo.sequence += 256;
		icmp.checksum = checksum((void *)&icmp, sizeof(struct icmphdr));
		sendto(g_ping.sock, &icmp, sizeof(icmp), 0, addr->ai_addr,
				addr->ai_addrlen);
		g_ping.sent += 1;
		
		char data[84] = {0};
		struct iovec iov = {.iov_base = &data, .iov_len = sizeof(data)};;
		struct msghdr msg = { .msg_iov = &iov, .msg_iovlen = -1 };
		int flag;
		recvmsg(g_ping.sock, &msg, flag);
		printf("%s\n", data);
		g_ping.time += 1;
		g_ping.recv += 1;
		sleep(1);
	}
	return (0);
}

void	ping(char verbose)
{
	int				gai;
	struct addrinfo	*res;

	if ((gai = getaddrinfo(g_ping.host, NULL, NULL, &res)) != 0)
	{
		printf("getaddrinfo: %s\n", gai_strerror(gai));
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
	ping_loop(res);
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
	ping(verbose);
	return (0);
}
