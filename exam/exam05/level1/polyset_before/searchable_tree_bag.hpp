#ifndef SEARCHABLE_TREE_BAG_HPP
# define SEARCHABLE_TREE_BAG_HPP

#include "searchable_bag.hpp"
#include "tree_bag.hpp"

class searchable_tree_bag : public tree_bag, public searchable_bag {
	private:


	public:
		searchable_tree_bag();
};


#endif
