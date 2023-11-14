#include "tag.hpp"



std::list<Tag> Tag::s_tags {};



bool Tag::doTagExist(const std::string &name)
{
	for (auto it {s_tags.begin()}; it != s_tags.end(); ++it)
	{
		if (it->name == name)
			return true;
	}

	return false;
}



int Tag::getAddressFromName(const std::string &name)
{
	for (auto it {s_tags.begin()}; it != s_tags.end(); ++it)
	{
		if (it->name == name)
			return it->address;
	}

	return -1;
}



void Tag::registerTag(const std::string &name, int address)
{
	s_tags.push_back({name, address});
}


