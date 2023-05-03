/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsimon <nsimon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 14:48:08 by nsimon            #+#    #+#             */
/*   Updated: 2023/05/03 18:10:43 by nsimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

#ifndef DEBUG
# define DEBUG 0
#endif

struct s_ping	g_ping = {
	.sock = 0,
	.host = NULL,
	.ip = NULL,
	.id = 0,
	.sent = 0,
	.recv = 0,
	.interval = 1,
	.res_addrinfo = NULL,
	.timeStats = NULL,
};

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

void	clear_all(struct s_ping *ping)
{
	if (ping->sock)
		close(ping->sock);
	if (ping->res_addrinfo)
		freeaddrinfo(ping->res_addrinfo);
	if (ping->timeStats)
		clear_time(&ping->timeStats);
	if (ping->pingData)
		clear_data(&ping->pingData);
}

void	sigint_handler(int signum)
{
	(void)signum;
	struct timeval now;
	struct s_ping *ping = &g_ping;

	gettimeofday(&now, NULL);
	printf("--- %s ping statistics ---\n", ping->host);
	printf("%d packets transmitted, %d packets received, %d%% packet loss\n",
		ping->sent,
		ping->recv,
		ping->sent == 0 ? 0 : (ping->sent - ping->recv) * 100 / ping->sent);
	printf("round-trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f ms\n",
		min(ping->timeStats),
		avg(ping->timeStats),
		max(ping->timeStats),
		stddev(ping->timeStats));
	clear_all(ping);
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

int ping_receive(struct s_ping *ping, int verbose) {
	t_recv buf = {0};
	struct iovec iov = { .iov_base = &buf, .iov_len = sizeof(buf) * 2 };
	struct msghdr msg = { .msg_iov = &iov, .msg_iovlen = 1 };
	ssize_t			ret;
	struct timeval	now, *timeSend;
	double diff;

	ret = recvmsg(ping->sock, &msg, 0);
	gettimeofday(&now, NULL);
	if (DEBUG)
	{
		printf("ret: %ld \t", ret);
		printf("errno: %d \t", errno);
		printf("strerror: %s \n", strerror(errno));
	}
	if (ret == -1) {
		if (errno == EAGAIN || errno == EWOULDBLOCK)
		{
			// if (now.tv_sec - timeSend->tv_sec > ping->interval)
			// {
			// 	if (verbose)
			// 		printf("Request timeout for icmp_seq %d\n", ping->sent);
			// 	ping->lost += 1;
			// 	return (0);
			// }
			return (1);
		}
		printf("recvmsg: %s\n", strerror(errno));
		return (1);
	}
	if (ret < sizeof(struct icmphdr)) {
		printf("recvmsg: too short packet\n");
		return (1);
	}
	if (DEBUG) {
		printf("type: %d \t", buf.icmp.type);
		printf("code: %d \t", buf.icmp.code);
		printf("id: %d \t", buf.icmp.un.echo.id);
		printf("seq: %d \t", buf.icmp.un.echo.sequence);
		printf("checksum: %d \t", buf.icmp.checksum);
		printf("ttl: %d \n", buf.ip.ttl);
	}
	if (buf.icmp.un.echo.id != g_ping.id) {
		return (1);
	}
	timeSend = get_data(ping->pingData, buf.icmp.un.echo.sequence);
	if (buf.icmp.type == ICMP_ECHOREPLY) {
		ping->recv += 1;
	}
	diff = (now.tv_sec - timeSend->tv_sec) * 1000 +
		(double)(now.tv_usec - timeSend->tv_usec) / 1000;
	printf("%ld bytes from %s: icmp_seq=%d ttl=%d time=%.3f ms \n",
		ret, ping->ip,
		buf.icmp.un.echo.sequence, buf.ip.ttl, diff);
	add_time(&ping->timeStats, diff);
	remove_data(&ping->pingData, buf.icmp.un.echo.sequence);
	return (0);
}

int	ping_loop(struct s_ping *ping, int verbose) {
	struct icmphdr	icmp = {0};
	ssize_t			res;
	struct timeval	time;

	icmp.type = ICMP_ECHO;
	icmp.un.echo.id = ping->id;
	icmp.un.echo.sequence = 0;
	for (;;) {
		gettimeofday(&time, NULL);
		icmp.un.echo.sequence++;
		icmp.checksum = 0;
		icmp.checksum = checksum((void *)&icmp, sizeof(icmp));
		res = sendto(ping->sock, &icmp, sizeof(icmp), 0, ping->res_addrinfo->ai_addr,
			ping->res_addrinfo->ai_addrlen);
		if (res == sizeof(icmp))
			ping->sent += 1;
		add_data(&ping->pingData, icmp.un.echo.sequence, time);
		if (DEBUG)
			printf("sendto: %s\n", strerror(errno));
		ping_receive(ping, verbose);
		sleep(ping->interval);
	}
}

int main(int argc, char **argv)
{
	char			verbose;
	int				gai;
	struct s_ping 	*ping;

	ping = &g_ping;
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
	ping->host = argv[argc - 1];

	/* Get addresse informations */
	if ((gai = getaddrinfo(ping->host, NULL, NULL, &ping->res_addrinfo)) != 0)
	{
		printf("ft_ping: %s\n", gai_strerror(gai));
		exit(1);
	}
	ping->sock = socket(AF_INET, SOCK_TYPE, IPPROTO_ICMP);
	if (ping->sock < 0)
	{
		// printf("ft_ping: %s\n", strerror(errno));
		printf("ft_ping: %s\n", "You must be root to run this program");
		exit(1);
	}
	struct timeval g_timeout = { .tv_sec = ping->interval, .tv_usec = 0 };
	setsockopt(ping->sock, SOL_SOCKET, SO_RCVTIMEO, (const char *)&g_timeout, sizeof(g_timeout));
	ping->ip = inet_ntoa(((struct sockaddr_in *)ping->res_addrinfo->ai_addr)->sin_addr);
	ping->id = getpid() & 0xFFFF;
	if (verbose)
		printf("FT_PING %s (%s) %ld data bytes, id 0x%x = %d\n", ping->host, ping->ip,
			sizeof(struct icmphdr) + sizeof(struct iphdr), ping->id, ping->id);
	else
		printf("FT_PING %s (%s) %ld data bytes\n", ping->host, ping->ip,
			sizeof(struct icmphdr) + sizeof(struct iphdr));
	signal(SIGINT, sigint_handler);

	ping_loop(ping, verbose);
	// g_ping.infos = NULL;
	// ping(verbose);
	sigint_handler(0);
	return (0);
}
