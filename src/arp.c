/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arp.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/09 14:38:18 by mamartin          #+#    #+#             */
/*   Updated: 2022/10/15 18:15:26 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <netpacket/packet.h>
#include <stdio.h>
#include <errno.h>

#include "ft_malcolm.h"

static int validate_arp_request(t_arpmsg* frame)
{
	return (
		frame->hdr.h_proto = ntohs(ETH_P_ARP) && // ARP protocol
		frame->data.arp_hrd == htons(ARPHRD_ETHER) &&
		frame->data.arp_pro == htons(ETH_P_IP) &&
		frame->data.arp_hln == ETHER_ADDR_LEN &&
		frame->data.arp_pln == sizeof(in_addr_t) &&
		frame->data.arp_op == htons(ARPOP_REQUEST) // request
	);
}

int wait_arp_request(int sock)
{
	static char buf[200] = { 0 };
	ssize_t bytes = recvfrom(sock, buf, sizeof(buf), 0, NULL, NULL);
	if (bytes == -1)
		return (errno == EAGAIN) ? 0 : -1;

	if ((size_t)bytes < sizeof(t_arpmsg))
		return 0; // ethernet frame is too small to hold ARP request
	return validate_arp_request((t_arpmsg*)buf);
}

int send_arp_reply(int sock, int ifindex, t_host* source, t_host* target)
{
	struct sockaddr_ll addr = {0};
	addr.sll_family = AF_PACKET;
	addr.sll_protocol = htons(ETH_P_ARP);
	addr.sll_ifindex = ifindex;
	addr.sll_halen = ETH_ALEN;
	ft_memcpy(addr.sll_addr, target->ether, ETH_ALEN);

	t_arpmsg reply = {0};
	/* Fill Ethernet header */
	ft_memcpy(reply.hdr.h_source, source->ether, ETH_ALEN);
	ft_memcpy(reply.hdr.h_dest, target->ether, ETH_ALEN);
	reply.hdr.h_proto = htons(ETH_P_ARP);
	/* Fill ARP reply */
	reply.data.arp_hrd = htons(ARPHRD_ETHER);
	reply.data.arp_pro = htons(ETH_P_IP);
	reply.data.arp_hln = ETHER_ADDR_LEN;
	reply.data.arp_pln = sizeof(in_addr_t);
	reply.data.arp_op = htons(ARPOP_REPLY);
	ft_memcpy(reply.data.arp_sha, source->ether, ETH_ALEN);
	ft_memcpy(reply.data.arp_spa, source->ip, sizeof(in_addr_t));
	ft_memcpy(reply.data.arp_tha, target->ether, ETH_ALEN);
	ft_memcpy(reply.data.arp_tpa, target->ip, sizeof(in_addr_t));
	
	ssize_t bytes = sendto(sock, &reply, sizeof(t_arpmsg), 0, (struct sockaddr*)&addr, sizeof(addr));
	return (bytes == sizeof(t_arpmsg)) ? 0 : -1;
}
