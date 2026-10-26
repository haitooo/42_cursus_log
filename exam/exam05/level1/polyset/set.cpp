#include "set.hpp"

set::set(searchable_bag &bag) : bag(bag) {}

set::set(const set &src) : bag(src.bag) {}

// bag は参照メンバで付け替えできないため、代入では何もしない
set &set::operator=(const set &src) {
	(void)src;
	return *this;
}

set::~set() {}

void set::insert(int value) {
	if (!bag.has(value))
		bag.insert(value);
}

void set::insert(int *values, int count) {
	for (int i = 0; i < count; i++)
		insert(values[i]);
}

bool set::has(int value) const {
	return bag.has(value);
}

void set::print() const {
	bag.print();
}

void set::clear() {
	bag.clear();
}

const searchable_bag &set::get_bag() const {
	return bag;
}
