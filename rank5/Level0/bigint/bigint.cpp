/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bigint.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcadinot <zcadinot@student.42lehavre.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 13:06:50 by zcadinot          #+#    #+#             */
/*   Updated: 2026/04/29 14:32:36 by zcadinot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bigint.hpp"

bigint::bigint ()
{
	this->_value = "0";
}

bigint::bigint(unsigned int num)
{
	std::stringstream ss;
	ss << num;
	this->_value = ss.str();
	// std::cout << "str: " << str << std::endl;
}

std::ostream &operator<<(std::ostream &out, const bigint &n)
{
	out << n._value;
	return (out);
}

unsigned int stringToUINT(std::string str)
{
	std::stringstream ss(str);
	unsigned int res;
	ss >> res;
	return (res);
}

bigint::bigint(const bigint &other)
{
	this->_value = other._value;
}

std::string reverse(const std::string str)
{
	std::string rev;
	size_t i = str.length();
	while (i > 0)
	{
		rev.push_back(str[i - 1]);
		i--;
	}
	return (rev);
}

std::string addition(const bigint &obj1, const bigint &obj2)
{
	std::string	str1 = reverse(obj1.get_value());
	std::string	str2 = reverse(obj2.get_value());
	std::string	res;
	size_t		i = 0;
	size_t		carry = 0;
	int nb1;
	int nb2;
	int sum;

	while (i < str1.length() || i < str2.length() || carry)
	{	
		nb1 = (i < str1.length()) ? str1[i] - '0' : 0;
		nb2 = (i < str2.length()) ? str2[i] - '0' : 0;
		sum = nb1 + nb2 + carry;
		carry = sum / 10;
		res.push_back((sum %10) + '0');
		i++;
	}
	return (reverse(res));
}

bigint bigint::operator+(const bigint &other) const 
{
	bigint temp(other);
	temp._value.clear();
	std::string res = addition(*this, other);
	temp._value = res;
	return (temp);
}

bigint &bigint::operator+=(const bigint &other)
{
	*this = *this + other;
	return (*this);
}

bigint &bigint::operator++()
{
	*this += bigint(1);
	return (*this);
}

bigint bigint::operator++(int) 
{
	bigint temp(*this);
	*this += bigint(1);
	return (temp);
}

bigint bigint::operator<<(const unsigned int n) const
{
	bigint temp(*this);
	temp._value.append(n, '0');
	return (temp);
}

bigint bigint::operator<<(const bigint& other)const
{
	bigint temp;
	temp = (*this) << stringToUINT(other._value);
	return(temp);
}

bigint &bigint::operator<<=(const unsigned int n)
{
	*this = *this << n;
	return (*this);
}

bigint &bigint::operator<<=(const bigint& other)
{
	*this = *this << stringToUINT(other._value);
	return (*this);
}

bigint bigint::operator>>(const unsigned int n) const
{
	bigint temp(*this);

	if (n >= temp._value.length())
		temp._value = "0";
	else
		temp._value.erase(temp._value.length() - n, n);
	return (temp);
}

bigint &bigint::operator>>=(const bigint &other)
{
	*this = *this >> stringToUINT(other._value);
	return (*this);
}

bigint bigint::operator>>(const bigint &other) const
{
	bigint temp;
	temp = *this >> stringToUINT(other._value);
	return (temp);
}

bool bigint::operator<(const unsigned int n)
{
	return ((stringToUINT(this->_value) < n));	
}


bool bigint::operator<(const bigint &other)
{
	return ((stringToUINT(this->_value) < stringToUINT(other._value)));	
}

bool bigint::operator>(const unsigned int n)
{
	return ((stringToUINT(this->_value) > n));	
}

bool bigint::operator>(const bigint &other)
{
	return ((stringToUINT(this->_value) > stringToUINT(other._value)));	
}

bool bigint::operator==(const unsigned int n)
{
	return ((stringToUINT(this->_value) == n));
}

bool bigint::operator==(const bigint &other)
{
	return ((stringToUINT(this->_value) == stringToUINT(other._value)));
}

bool bigint::operator!=(const unsigned int n)
{
	return((stringToUINT(this->_value) != n));
}

bool bigint::operator!=(const bigint &other)
{
	return((stringToUINT(this->_value) != stringToUINT(other._value)));
}

bool bigint::operator<=(const unsigned int n)
{
	return ((stringToUINT(this->_value) <= n));
}

bool bigint::operator<=(const bigint &other)
{
	return ((stringToUINT(this->_value) <= stringToUINT(other._value)));
}

bool bigint::operator>=(const unsigned int n)
{
	return ((stringToUINT(this->_value) >= n));
}

bool bigint::operator>=(const bigint &other)
{
	return ((stringToUINT(this->_value) >= stringToUINT(other._value)));
}



std::string bigint::get_value() const
{
	return (this->_value);
}
