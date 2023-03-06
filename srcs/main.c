/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsimon <nsimon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 14:48:08 by nsimon            #+#    #+#             */
/*   Updated: 2023/03/06 15:24:04 by nsimon           ###   ########.fr       */
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
	struct timeval now;

	gettimeofday(&now, NULL);
	printf("--- %s ping statistics ---\n", g_ping.host);
	printf("%d packets transmitted, %d received, %d%% packet loss\n",
		g_ping.sent,
		g_ping.recv,
		(g_ping.sent - g_ping.recv) * 100 / g_ping.sent);
	close(g_ping.sock);
	exit(0);
}

unsigned short checksum(unsigned short *addr, size_t size)
{
	register long sum = 0;
	for (; size > 1; size -= 2)
		sum += *addr++;
	if (size > 0)
		sum += *(unsigned char *)addr;
	while (sum >> 16)
		sum = (sum & 0xffff) + (sum >> 16);
	return (~sum);
}

g_ping_info *find_ping(int seq)
{
	g_ping_info *tmp = g_ping.infos;

	while (tmp)
	{
		if (tmp->seq == seq)
		{
			gettimeofday(&tmp->tv_recv, NULL);
			return (tmp);
		}
		tmp = tmp->next;
	}
	return (NULL);
}

void clear_ping(void)
{
	g_ping_info *tmp = g_ping.infos;
	struct timeval now;

	gettimeofday(&now, NULL);
	while (tmp)
	{
		if (now.tv_sec - tmp->tv_send.tv_sec > g_ping.ttl)
		{
			g_ping.infos = tmp->next;
			free(tmp);
			tmp = g_ping.infos;
		}
	}
	free(tmp);
}

int ping_receive(void)
{
	char			buf[1024];
	struct iphdr	*ip;
	struct icmphdr	*icmp;
	int				len;
	g_ping_info		*tmp;
	double			time;

	len = recvfrom(g_ping.sock, buf, sizeof(buf), 0, NULL, NULL);
	if (len < 0)
	{
		printf("recvfrom: %s\n", strerror(errno));
		return (1);
	}
	ip = (struct iphdr *)buf;
	icmp = (struct icmphdr *)(buf + (ip->ihl << 2));
	if (icmp->type == ICMP_ECHOREPLY)
	{
		if ((tmp = find_ping(icmp->un.echo.sequence)) == NULL)
			return (1);
		time = (tmp->tv_recv.tv_sec - tmp->tv_send.tv_sec) * 1000 +
			(double)(tmp->tv_recv.tv_usec - tmp->tv_send.tv_usec) / 1000;
		printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%.3f ms \n",
			len, g_ping.ip,
			icmp->un.echo.sequence, ip->ttl + icmp->un.echo.sequence, time);
	}
	// clear_ping();
	return (0);
}

int ping_loop(struct addrinfo *addr)
{
	struct icmphdr icmp = {0};
	g_ping_info *last = NULL;

	icmp.type = ICMP_ECHO;
	icmp.un.echo.id = getpid();
	icmp.un.echo.sequence = 0;
	while (1)
	{
		icmp.un.echo.sequence += 1;
		icmp.checksum = 0;
		icmp.checksum = checksum((void *)&icmp, sizeof(struct icmphdr));
		sendto(g_ping.sock, &icmp, sizeof(icmp), 0, addr->ai_addr,
				addr->ai_addrlen);
		if (last) {
			last->next = malloc(sizeof(g_ping_info));
			last = last->next;
		}
		else {
			g_ping.infos = malloc(sizeof(g_ping_info));
			last = g_ping.infos;
		}
		gettimeofday(&last->tv_send, NULL);
		last->seq = icmp.un.echo.sequence;
		g_ping.sent += 1;
		if (ping_receive() == 0)
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
	g_ping.ip = inet_ntoa(((struct sockaddr_in *)res->ai_addr)->sin_addr);
	printf("PING %s (%s) 56 bytes of data.\n", g_ping.host, g_ping.ip);
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
