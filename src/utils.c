/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/09 00:23:48 by mamartin          #+#    #+#             */
/*   Updated: 2022/10/09 03:37:26 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "ft_malcolm.h"

int failure(const char* msg)
{
	ft_putendl_fd(msg, STDERR_FILENO);
	return EXIT_FAILURE;
}

int print_usage(const char* program_name)
{
	dprintf(STDERR_FILENO,
		"Usage: %s [source IP] [source MAC] [target IP] [target MAC]\n",
		program_name);
	return EXIT_FAILURE;
}
