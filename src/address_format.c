/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   address_format.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/07 16:04:21 by mamartin          #+#    #+#             */
/*   Updated: 2022/10/10 22:14:07 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <netinet/if_ether.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <stdio.h>
#include "ft_malcolm.h"

int mac_addr_to_array(const char* addr, uint8_t* buf, uint8_t bufsize)
{
	static const char* hex = "0123456789abcdef";
	static const uint8_t base = 16;

	int i, j;
	int nsections = 0;
	int start = 0;

	if (bufsize < ETH_ALEN)
		return -1;

	for (i = 0; addr[i]; i++)
	{
		if (addr[i] == ':')
		{
			if (++nsections == ETH_ALEN || i - start < 2)
				return -1;
			start = i + 1;
			buf++;
		}
		else if (i - start >= 2)
			return -1;
		else
		{
			for (j = 0; hex[j] && hex[j] != addr[i]; j++);
			if (j == base)
				return -1;
			*buf = *buf * base + j;
		}
	}
	return (nsections == ETH_ALEN - 1 && i - start == 2) ? 0 : -1;
}

int ip_addr_to_array(const char* addr, uint8_t* buf, uint8_t bufsize)
{
	if (bufsize < sizeof(in_addr_t))
		return -1;

	in_addr_t binary_addr = inet_addr(addr);
	if (binary_addr == INADDR_NONE)
		return -1;

	ft_memcpy(buf, &binary_addr, sizeof(in_addr_t));
	return 0;
}

void print_mac_addr(uint8_t* buf)
{
	int i;
	for (i = 0; i < ETH_ALEN; i++)
		printf("%02X%c", buf[i], (i != ETH_ALEN - 1) ? ':' : '\0');
}

void print_ip_addr(uint8_t* buf)
{
	unsigned int i;
	for (i = 0; i < sizeof(in_addr_t); i++)
		printf("%d%c", buf[i], (i != sizeof(in_addr_t) - 1) ? '.' : '\0');
}
