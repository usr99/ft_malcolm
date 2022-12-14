/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/09 00:23:48 by mamartin          #+#    #+#             */
/*   Updated: 2022/10/12 13:35:37 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "ft_malcolm.h"

void failure(const char* msg, int sck)
{
	ft_putendl_fd(msg, STDERR_FILENO);
	if (sck != -1)
		close(sck);
	exit(EXIT_FAILURE);
}

void print_usage(const char* program_name)
{
	dprintf(STDERR_FILENO,
		"Usage: %s [source IP] [source MAC] [target IP] [target MAC]\n",
		program_name);
	exit(EXIT_FAILURE);
}
