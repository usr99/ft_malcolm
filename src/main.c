/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 19:03:49 by mamartin          #+#    #+#             */
/*   Updated: 2022/10/09 03:53:24 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <ifaddrs.h>
#include <linux/if_link.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "ft_malcolm.h"

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

static int create_arp_socket(int ifindex)
{
	int fd = socket(AF_PACKET, SOCK_DGRAM, htons(ETH_P_ARP));
	if (fd == -1)
		return -1;

	struct sockaddr_ll addr;
	ft_memset(&addr, 0, sizeof(struct sockaddr_ll));
	addr.sll_family = AF_PACKET;
	addr.sll_ifindex = ifindex;
	addr.sll_protocol = htons(ETH_P_ARP);

	if (bind(fd, (struct sockaddr*)&addr, sizeof(struct sockaddr_ll)) == -1)
	{
		close(fd);
		return -1;
	}
	return fd;
}

int main(int argc, char **argv)
{
	if (argc != 5)
		return print_usage(argv[0]);

	t_host source;
	t_host target;
	if (validate_arguments(argv, &source, &target) == -1)
		return EXIT_FAILURE;

	if (getuid() != 0)
		return failure("root privileges are needed to create packet socket");

	int ifindex = get_interface_index();
	if (ifindex == -1)
		return failure("failed to retrieve network interface");

	int sock = create_arp_socket(ifindex);
	if (sock == -1)
		return failure("failed to create ARP socket");

	// char buf[200] = { 0 };
	// ssize_t bytes = recvfrom(sock, buf, sizeof(buf), 0, NULL, NULL);
	// if (bytes == -1)
	// {
	// 	close(sock);
	// 	failure("failed to create ARP socket");
	// }

	// struct ether_arp* reply = (struct ether_arp*)buf;
	// printf(
	// 	"received %ld bytes %s: MAC address = %d %02x:%02x:%02x:%02x:%02x:%02x\n",
	// 	bytes, 
	// 	ntohs(reply->arp_op),
	// 	reply->arp_sha[0],	
	// 	reply->arp_sha[1],	
	// 	reply->arp_sha[2],	
	// 	reply->arp_sha[3],
	// 	reply->arp_sha[4],
	// 	reply->arp_sha[5]
	// );

	// listen ARP request
	// send ARP reply

	// struct sockaddr_ll addr;
	// ft_memset(&addr, 0, sizeof(struct sockaddr_ll));
	// addr.sll_family = AF_PACKET;
	// addr.sll_protocol = htons(ETH_P_ARP);
	// addr.sll_ifindex = 2;
	// addr.sll_halen = ETH_ALEN;
	// ft_memset(addr.sll_addr, 0xFF, ETH_ALEN);

	// struct ether_arp req;
	// req.arp_hrd = htons(ARPHRD_ETHER);
	// req.arp_pro = htons(ETH_P_IP);
	// req.arp_hln = ETHER_ADDR_LEN;
	// req.arp_pln = sizeof(in_addr_t);
	// req.arp_op = htons(ARPOP_REQUEST);
	// ft_memset(&req.arp_tha, 0, sizeof(req.arp_tha));

	// /* Source */
	// if (mac_addr_to_array(argv[2], req.arp_sha, ETH_ALEN) == -1)
	// 	failure("invalid MAC address");
	// if (ip_addr_to_array(argv[1], req.arp_spa, sizeof(in_addr_t)) == -1)
	// 	failure("invalid MAC address");

	// /* Target */
	// if (ip_addr_to_array(argv[3], req.arp_tpa, sizeof(in_addr_t)) == -1)
	// 	failure("invalid MAC address");

	// ssize_t bytes = sendto(sckfd, &req, sizeof(req), 0, (struct sockaddr*)&addr, sizeof(addr));
	// if (bytes <= 0)
	// 	fatal("sendto");

	close(sock);
	return EXIT_SUCCESS;
}