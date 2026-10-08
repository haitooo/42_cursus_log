#ifndef ITER_HPP
# define ITER_HPP

#include <cstddef>

template <typename T, typename F>
void	iter(T* array, std::size_t const length, F func)
{
	if (array == NULL)
		return ;
	for (std::size_t i = 0; i < length; i++)
		func(array[i]);
}

#endif
