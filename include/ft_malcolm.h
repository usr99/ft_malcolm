/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malcolm.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/09 00:30:59 by mamartin          #+#    #+#             */
/*   Updated: 2022/10/12 13:44:47 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _FT_MALCOLM_H_
# define _FT_MALCOLM_H_

# ifndef __USE_MISC
#  define __USE_MISC
# endif

# include <stdint.h>
# include <netinet/if_ether.h>
# include <netinet/ip.h>

# include "libft.h"

typedef struct s_host
{
	uint8_t ip[sizeof(in_addr_t)];
	uint8_t ether[ETH_ALEN];
} t_host;

typedef struct s_arpmsg
{
	struct ethhdr hdr;
	struct ether_arp data;
} t_arpmsg;

/* arp.c */
int wait_arp_request(int sock, t_host* source, t_host* target);
int send_arp_reply(int sock, int ifindex, t_host* source, t_host* target);

/* address_format.c */
int mac_addr_to_array(const char* addr, uint8_t* buf, uint8_t bufsize);
int ip_addr_to_array(const char* addr, uint8_t* buf, uint8_t bufsize);
void print_mac_addr(uint8_t* buf);
void print_ip_addr(uint8_t* buf);

/* utils.c */
void failure(const char* msg, int sck);
void print_usage(const char* program_name);

#endif
