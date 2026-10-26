#ifndef VECT2_HPP
# define VECT2_HPP

#include <algorithm>
#include <iostream>

class vect2 {
	private:
		int x;
		int y;

	public:
		vect2();
		vect2(int n1, int n2);
		vect2(const vect2& other);
		vect2& operator=(const vect2& other);
		~vect2();

		int getX() const;
		int getY() const;

		int		operator[](int i) const;
		int&	operator[](int i);

		vect2&	operator++();
		vect2	operator++(int);
		vect2&	operator--();
		vect2	operator--(int);

		vect2	operator+(const vect2& rhs) const;
		vect2	operator+(int n) const;
		vect2	operator+() const;
		vect2	operator-(const vect2& rhs) const;
		vect2	operator-(int n) const;
		vect2	operator-() const;
		vect2	operator*(const vect2& rhs) const;
		vect2	operator*(int n) const;

		vect2&	operator+=(const vect2& rhs);
		vect2&	operator+=(int n);
		vect2&	operator-=(const vect2& rhs);
		vect2&	operator-=(int n);
		vect2&	operator*=(const vect2& rhs);
		vect2&	operator*=(int n);

		bool	operator==(const vect2& rhs) const;
		bool	operator!=(const vect2& rhs) const;

};

std::ostream& operator<<(std::ostream& os, const vect2& v2);
vect2 operator+(int i, const vect2& v2);
vect2 operator-(int i, const vect2& v2);
vect2 operator*(int i, const vect2& v2);

#endif
