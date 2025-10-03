#include "Harl.hpp"

void	Harl::debug(void)
{
	std::cout << "\033[33mDEBUG  :\033[0m\033[1;3;31mElectric scooters really shouldn't be on the roadway. It just doesn't make sence!\033[0m" << std::endl;
}

void	Harl::info(void)
{
	std::cout << "\033[33mINFO   :\033[0m\033[1;3;31mIt's dangerous both for people riding scooters and for thoes driving cars. Neither is good for anyone's safety!\033[0m" << std::endl;
}

void	Harl::warning(void)
{
	std::cout << "\033[33mWARNING:\033[0m\033[1;3;31mScooters should either stick to the sidewalks or better yet, we need dedicated lanes for them.\033[0m" << std::endl;
}

void	Harl::error(void)
{
	std::cout << "\033[33mERROR  :\033[0m\033[1;3;31mThis is outrageous! I'm furious! Call the police right now!!\033[0m" << std::endl;
}

void	Harl::complain(const std::string& level)
{
	int			i = 0;
	std::string levels[] = {"DEBUG", "INFO", "WARNING", "ERROR", "OTHERS"};

	while (i < 5)
	{
		if (levels[i] == level || i == 4)
		{
			switch (i)
			{
				case 0:debug();
					//fall through
				case 1:info();
					//fall through
				case 2:warning();
					//fall through
				case 3:error();
					break ;
				default:
					std::cout << "\033[32m        [ Probably complaining about insignificant problems ]\033[0m" << std::endl;
			}
			return ;
		}
		i++;
	}
}
