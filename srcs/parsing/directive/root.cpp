/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   root.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 16:52:14 by spayeur           #+#    #+#             */
/*   Updated: 2023/01/22 12:29:55 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <utility>
#include <string>
#include <limits.h>
#include <unistd.h>
#include <pwd.h>
#include "Http_block.hpp"
#include "Server_block.hpp"
#include "Location_block.hpp"
#include "parsing.hpp"

int	parse_root(std::pair<e_context, void*> &context, const std::string &directive, const std::vector<std::string> &args, const size_t l)
{
	// Construct path
	const struct passwd	*pws = getpwuid(geteuid());
	const std::string	username = !(pws) ? "root" : pws->pw_name;
	const std::string	working_directory = "/goinfre/" + username + "/webserv/html";
	std::string			root;
	std::string			path;

	if (args[0][0] == '/')
		path = args[0];
	else
		path = working_directory + '/' + args[0];

	// Normalize path
	root = normalize_path(directive, l, path, true, true);
	if (root.empty())
		return (-1);
	if (root.size() >= PATH_MAX)
		return (too_long_path_after_resolution_error(directive, l, root.substr(0, 10) + "..."));

	if (context.first == HTTP)
	{
		Http_block	&http = get_context<Http_block>(context);

		http.root = root;
		http.set_flag_root(true);
	}
	else if (context.first == SERVER)
	{
		Server_block	&server = get_context<Server_block>(context);

		server.root = root;
		server.set_flag_root(true);
	}
	else
	{
		Location_block	&location = get_context<Location_block>(context);

		location.root = root;
		location.set_flag_root(true);
	}
	return (0);
}
