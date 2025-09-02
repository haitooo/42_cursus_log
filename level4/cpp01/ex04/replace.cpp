#include <string>
#include <iostream>
#include <fstream>

int	replace(const std::string& filename, const std::string& s1, const std::string& s2)
{
	std::ifstream input(filename.c_str());
	if (!input)
	{
		std::cerr << "Error: cannot open input file\n";
		return (1);
	}
	std::ofstream outfile((filename + ".replace").c_str());
	if (!outfile)
	{
		std::cerr << "Error: cannot open output file\n";
		return(1);
	}

	std::string						line;
	std::string::size_type			i;
	while (std::getline(input, line))
	{
		i = 0;
		while (i < line.size())
		{
			if (line[i] == s1[0])
			{
				std::string tmp = line.substr(i, s1.size());
				if (tmp == s1)
				{
					outfile << s2;
					i += s1.size();
				}
				else
					outfile << line[i++];
			}
			else
				outfile << line[i++];
		}
		outfile << std::endl;
	}
	return(0);
}
