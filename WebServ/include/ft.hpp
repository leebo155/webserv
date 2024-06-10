#ifndef FT_HPP
# define FT_HPP

# include <string>
# include <vector>
# include <exception>

# define WHITE_SPACE " \t\n\r\f\v"

namespace Ft
{
	std::string	trim(std::string & str);
	std::vector<std::string> split(std::string & str);
	std::vector<std::string> split(std::string & str, std::string & delim);
int dectoi(std::string dec);
}

#endif
