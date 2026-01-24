#ifndef CHARACTER_HPP
# define CHARACTER_HPP

#include "ICharacter.hpp"
#include "AMateria.hpp"

class	Character : public ICharacter
{
	private:
		std::string	name;
		AMateria*	inventory[4];
	public:
		Character();
		Character(const std::string& name);
		Character(const Character& other);
		Character&	operator=(const Character& other);
		virtual ~Character();
		
		std::string const &	getName() const; /*override*/

		AMateria*	getEquipment(int idx) const;

		void	equip(AMateria* m); /*override*/
		void	unequip(int idx); /*override*/
		void	use(int idx, ICharacter& target); /*override*/
		int		countEmptySlots() const;
};

#endif
