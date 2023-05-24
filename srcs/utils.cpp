/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avillar <avillar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 16:11:05 by avillar           #+#    #+#             */
/*   Updated: 2023/05/24 16:39:53 by avillar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/utils.hpp"


std::string	merge(std::vector<std::string> v, int start)
{
	std::string s;

	v[3] = v[3].substr(1);
	if (start == (int)v.size())
		return (v[3]);
	for (int i = start - 1; i < (int)v.size(); i++)
	{
		s += v[i];
		if (i != (int)v.size() - 1)
			s += " ";
	}
	return (s);
}
