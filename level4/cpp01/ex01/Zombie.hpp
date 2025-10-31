#ifndef ZOMBIE_HPP
# define ZOMBIE_HPP

# include <string>
# include <iostream>

class Zombie
{
	private:
		std::string name;
	
	public:
		Zombie();
		Zombie(std::string const& name);
		~Zombie();

		void	announce() const;
		void	setName(std::string const& newName);
};

Zombie* zombieHorde(int N, std::string name);

#endif
