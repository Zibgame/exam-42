/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vect2.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcadinot <zcadinot@student.42lehavre.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 15:23:35 by zcadinot          #+#    #+#             */
/*   Updated: 2026/04/29 17:04:28 by zcadinot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vect2.hpp"

std::ostream &operator<<(std::ostream &cout, const vect2 &obj)
{
	cout << "{" << obj._x << ", " << obj._y << "}";
	return (cout);
}

vect2 operator*(int i, const vect2 &obj)
{
	return (vect2(obj._x * i, obj._y * i));
}

vect2::vect2()
{
	this->_x = 0;
	this->_y = 0;
}

vect2::vect2(int x, int y)
{
	this->_x = x;
	this->_y = y;
}

vect2::vect2(const vect2 &other)
{
	this->_x = other._x;
	this->_y = other._y;
}

vect2 &vect2::operator=(const vect2 &other)
{
	if (this != &other)
	{
		this->_x = other._x;
		this->_y = other._y;
	}
	return (*this);
}

int &vect2::operator[](int index)
{
	if (index == 1)
		return (this->_y);
	return (this->_x);
}

int vect2::operator[](int index) const
{
	if (index == 1)
		return (this->_y);
	return (this->_x);
}

vect2 vect2::operator+(const vect2 &other) const
{
	return (vect2(this->_x + other._x, this->_y + other._y));
}

vect2 vect2::operator++()
{
	this->_x++;	
	this->_y++;	
	return (*this);
}

vect2 vect2::operator++(int)
{
	this->_x++;	
	this->_y++;	
	return (*this);
}

vect2 vect2::operator-(const vect2 &other)
{
	return (vect2(this->_x - other._x, this->_x - other._x));
}

vect2 vect2::operator-()
{
	this->_x *= -1;
	this->_y *= -1;
	return (*this);
}

vect2 vect2::operator--()
{
	this->_x--;	
	this->_y--;	
	return (*this);
}

vect2 vect2::operator--(int)
{
	this->_x--;	
	this->_y--;	
	return (*this);
}

vect2 vect2::operator+=(const vect2 &other)
{
	this->_x += other._x;	
	this->_y += other._y;	
	return (*this);
}

vect2 vect2::operator-=(const vect2 &other)
{
	this->_x -= other._x;	
	this->_y -= other._y;	
	return (*this);
}

vect2 vect2::operator*(const int i) const
{
	return (vect2(this->_x * i, this->_y * i));
}

vect2 vect2::operator*=(const int i)
{
	this->_x *= i;
	this->_x *= i;
	return (*this);
}

bool vect2::operator==(const vect2 &other)
{
	return (((this->_x == other._x) && (this->_y == other._y)));
}

bool vect2::operator!=(const vect2 &other)
{
	return (this->_x != other._x || this->_y != other._y);
}
