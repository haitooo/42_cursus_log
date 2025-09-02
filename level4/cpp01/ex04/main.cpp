#include <string>
#include <iostream>
#include <fstream>
#include <sys/stat.h>

int	replace(const std::string& filename, const std::string& s1, const std::string& s2);

static bool is_regular_file(const std::string& path)
{
	struct stat st;

	if (stat(path.c_str(), &st) != 0)
		return (false);
	return (S_ISREG(st.st_mode) != 0);
}

int	check_error(const std::string& filename, const std::string& s1)
{
	if (filename.empty())
	{
		std::cerr << "Error: filename must not be empty.\n";
		return (1);
	}
	if (!is_regular_file(filename))
	{
		std::cerr << "Error: isnot regular file.\n";
		return (1);
	}
	if (s1.empty())
	{
		std::cerr << "Error: s1 must not be empty.\n";
		return (1);
	}
	if (s1.find('\n') != std::string::npos || s1.find('\r') != std::string::npos)
	{
		std::cerr << "Error: s1 has breakline.\n";
		return (1);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	if (argc != 4)
	{
		std::cerr << "Usage: ./replace <file> <s1> <s2>\n";
		return (1);
	}

	std::string	filename = argv[1];
	std::string	s1 = argv[2];
	std::string	s2 = argv[3];
	
	if (check_error(filename, s1) == 1)
		return (1);
	return (replace(filename, s1, s2));
}
