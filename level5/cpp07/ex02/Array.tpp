#ifndef ARRAY_TPP
# define ARRAY_TPP

#include "Array.hpp"

template <typename T>
Array<T>::Array() : elements(NULL), length(0)
{
}

template <typename T>
Array<T>::Array(unsigned int n) : elements(NULL), length(n)
{
	if (n > 0)
		elements = new T[n]();
}

template <typename T>
Array<T>::Array(const Array& other) : elements(copyElements(other)), length(other.length)
{
}

template <typename T>
Array<T>&	Array<T>::operator=(const Array& other)
{
	if (this == &other)
		return (*this);

	T*	copied = copyElements(other);

	delete[] elements;
	elements = copied;
	length = other.length;

	return (*this);
}

template <typename T>
Array<T>::~Array()
{
	delete[] elements;
}

template <typename T>
T&	Array<T>::operator[](unsigned int index)
{
	if (index >= length)
		throw OutOfBoundsException();

	return (elements[index]);
}

template <typename T>
T const &	Array<T>::operator[](unsigned int index) const
{
	if (index >= length)
		throw OutOfBoundsException();

	return (elements[index]);
}

template <typename T>
unsigned int	Array<T>::size() const
{
	return (length);
}

template <typename T>
T*	Array<T>::copyElements(const Array& other)
{
	if (other.length == 0)
		return (NULL);

	T*	copied = new T[other.length];

	try
	{
		for (unsigned int i = 0; i < other.length; i++)
			copied[i] = other.elements[i];
	}
	catch (...)
	{
		delete[] copied;
		throw ;
	}

	return (copied);
}

template <typename T>
const char*	Array<T>::OutOfBoundsException::what() const throw()
{
	return ("Array: index is out of bounds");
}

#endif
