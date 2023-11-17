#pragma once

#include <list>
#include <string>


struct Tag
{
	std::string name;
	int address;

	static bool doTagExist(const std::string &name);
	static int getAddressFromName(const std::string &name);
	static void registerTag(const std::string &name, int address);

	private:
		static std::list<Tag> s_tags;
};