#include "Harl.hpp"

int	main(void)
{
	Harl harl;

	std::cout << "[DEBUG]" << std::endl;
	harl.complain("DEBUG");
	std::cout << "\n[INFO]" << std::endl;
	harl.complain("INFO");
	std::cout << "\n[WARNIG]" << std::endl;
	harl.complain("WARNING");
	std::cout << "\n[ERROR]" << std::endl;
	harl.complain("ERROR");
	std::cout << "\n[HAPPY]" << std::endl;
	harl.complain("HAPPY");

	return (0);
}
