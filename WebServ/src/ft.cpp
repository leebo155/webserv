#include "ft.hpp"

std::string ft::trim(std::string & str)
{
	str.erase(0, str.find_first_not_of(WHITE_SPACE));
	str.erase(str.find_last_not_of(WHITE_SPACE) + 1);
	return str;
}

std::vector<std::string> ft::split(std::string & str)
{
	return ft::split(str, WHITE_SPACE);
}

std::vector<std::string> ft::split(std::string & str, std::string & delim)
{
	std::vector<std::string> ret;
	str.erase(0, str.find_first_not_of(delim));
	while (!str.empty)
	{
		ret.push_back(str.substr(0, str.find_first_of(delim)));
		str.erase(0, ret.back().size());
		str.erase(0, str.find_first_not_of(delim));
	}
	return ret;
}

int	ft::dectoi(std::string dec)
{
	long long	ret = 0;
	int			minus = 1;
	size_t		idx = 0;

	ft::trim(dec);

	while (dec[idx] && (dec[idx] == '-' || dec[idx] == '+')
	{
		if (dec[idx] == '-')
			minus *= -1;
		idx++;
	}

	while (dec[idx] && dec[idx] >= '0' && dec[idx] <= '9')
	{
		ret *= 10;
		ret += (dec[idx] - '0') * minus;
		if (ret < INT_MIN || ret > INT_MAX)
			throw static_cast<int>(ret / 10);
	}

	if (idx != dec.size())
		throw static_cast<int>(ret);

	return static_cast<int>(ret);
}
