/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avillar <avillar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 16:11:05 by avillar           #+#    #+#             */
/*   Updated: 2023/05/24 16:28:12 by avillar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/utils.hpp"


std::string	merge(std::vector<std::string> v, int start)
{
	std::string s;

	if (start == (int)v.size())
		return (v[4]);
	for (int i = start; i < (int)v.size(); i++)
	{
		s += v[i];
		if (i != (int)v.size() - 1)
			s += " ";
	}
	return (s);
}
