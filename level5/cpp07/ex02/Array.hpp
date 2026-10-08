#ifndef ARRAY_HPP
# define ARRAY_HPP

#include <cstddef>
#include <exception>

template <typename T>
class	Array {
	private:
		T*				elements;
		unsigned int	length;

		static T*	copyElements(const Array& other);

	public:
		Array();
		Array(unsigned int n);
		Array(const Array& other);
		Array&	operator=(const Array& other);
		~Array();

		T&				operator[](unsigned int index);
		T const &		operator[](unsigned int index) const;
		unsigned int	size() const;

		class	OutOfBoundsException : public std::exception {
			public:
				virtual const char*	what() const throw();
		};
};

#include "Array.tpp"

#endif
