/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bigint.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcadinot <zcadinot@student.42lehavre.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 13:09:02 by zcadinot          #+#    #+#             */
/*   Updated: 2026/04/28 17:02:45 by zcadinot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#pragma once 

#include <string>
#include <iostream>
#include <sstream>

class bigint
{
	public:
		bigint();
		bigint(const unsigned int n);
		bigint(const bigint &other);

		bigint operator+(const bigint &other) const;
		bigint &operator+=(const bigint &other);

		bigint &operator++();
		bigint operator++(int);

		bigint operator<<(const unsigned int n) const;
		bigint operator<<(const bigint &other) const;
		bigint operator>>(const unsigned int n) const;

		bigint &operator<<=(const unsigned int n);
		bigint &operator<<=(const bigint &other);
		bigint &operator>>=(const unsigned int n);
		bigint &operator>>=(const bigint &other);
	
		std::string get_value() const;
		friend std::ostream &operator<<(std::ostream &out, const bigint &n);

	private:
		std::string _value;
};
