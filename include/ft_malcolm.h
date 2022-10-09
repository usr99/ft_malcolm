/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malcolm.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/09 00:30:59 by mamartin          #+#    #+#             */
/*   Updated: 2022/10/09 03:55:15 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _FT_MALCOLM_H_
# define _FT_MALCOLM_H_

# include <stdint.h>
# include <netinet/if_ether.h>
# include <netinet/ip.h>

# include "libft.h"

typedef struct s_host
{
	uint8_t ip[sizeof(in_addr_t)];
	uint8_t ether[ETH_ALEN];
} t_host;

/* address_format.c */
int mac_addr_to_array(const char* addr, uint8_t* buf, uint8_t bufsize);
int ip_addr_to_array(const char* addr, uint8_t* buf, uint8_t bufsize);

/* utils.c */
int failure(const char* msg);
int print_usage(const char* program_name);

#endif
