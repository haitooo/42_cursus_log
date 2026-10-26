#ifndef SET_HPP
# define SET_HPP

#include "searchable_bag.hpp"

class set {
	private:
		searchable_bag &bag;

		set();

	public:
		set(searchable_bag &bag);
		set(const set &src);
		set &operator=(const set &src);
		~set();

		void insert(int value);
		void insert(int *values, int count);
		bool has(int value) const;
		void print() const;
		void clear();

		const searchable_bag &get_bag() const;
};

#endif
