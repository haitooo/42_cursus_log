#ifndef EASYFIND_TPP
# define EASYFIND_TPP

#include "easyfind.hpp"

template <typename T>
typename T::iterator	easyfind(T & container, int value)
{
	typename T::iterator	it = std::find(container.begin(), container.end(), value);

	if (it == container.end())
		throw std::runtime_error("easyfind: value not found");

	return (it);
}

template <typename T>
typename T::const_iterator	easyfind(T const & container, int value)
{
	typename T::const_iterator	it = std::find(container.begin(), container.end(), value);

	if (it == container.end())
		throw std::runtime_error("easyfind: value not found");

	return (it);
}

#endif
