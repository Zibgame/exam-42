/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vect2.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcadinot <zcadinot@student.42lehavre.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 15:23:49 by zcadinot          #+#    #+#             */
/*   Updated: 2026/04/29 17:03:40 by zcadinot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>

class vect2
{
	private:
		int _x;
		int _y;

	public:
		vect2();
		vect2(int x, int y);
		vect2(const vect2 &other);
		vect2 &operator=(const vect2 &other);
		int &operator[](int index);
		int operator[](int index) const;

		vect2 operator+(const vect2 &other) const;

		vect2 operator++();
		vect2 operator++(int);


		vect2 operator-(const vect2 &other);
		vect2 operator-();

		vect2 operator--();
		vect2 operator--(int);

		vect2 operator+=(const vect2 &other);

		vect2 operator-=(const vect2 &other);

		vect2 operator*(const int i) const;
		friend vect2 operator*(int i, const vect2 &obj);

		vect2 operator*=(const int i);

		bool operator==(const vect2 &other);

		bool operator!=(const vect2 &other);

		friend std::ostream &operator<<(std::ostream &cout, const vect2 &obj);
};
