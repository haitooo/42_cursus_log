#include "searchable_tree_bag.hpp"
#include <cstddef>

searchable_tree_bag::searchable_tree_bag() : tree_bag() {}

searchable_tree_bag::searchable_tree_bag(const searchable_tree_bag &src)
	: bag(), tree_bag(src), searchable_bag() {}

searchable_tree_bag &searchable_tree_bag::operator=(const searchable_tree_bag &src) {
	if (this != &src)
		tree_bag::operator=(src);
	return *this;
}

searchable_tree_bag::~searchable_tree_bag() {}

bool searchable_tree_bag::has(int value) const {
	node *current = tree;
	while (current != NULL) {
		if (value < current->value)
			current = current->l;
		else if (value > current->value)
			current = current->r;
		else
			return true;
	}
	return false;
}
