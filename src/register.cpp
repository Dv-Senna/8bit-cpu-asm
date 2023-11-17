#include <algorithm>
#include <map>

#include "register.hpp"



RegisterID getRegisterIDFromName(std::string name)
{
	static std::map<std::string, RegisterID> ids {
		{"a", RegisterID::A},
		{"b", RegisterID::B},
		{"c", RegisterID::C},
		{"d", RegisterID::D},
		{"e", RegisterID::E},
		{"f", RegisterID::F},
		{"h", RegisterID::H},
		{"l", RegisterID::L},
	};

	std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) {return std::tolower(c);});

	auto it {ids.find(name)};
	if (it == ids.end())
		return RegisterID::invalid;

	return it->second;
}