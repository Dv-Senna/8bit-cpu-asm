#include <iostream>

#include "error.hpp"



std::list<std::string> Error::s_errorMessages {};



void Error::pushErrorToStack(const std::string &errorMessage) noexcept
{
	s_errorMessages.push_back(errorMessage);
}



bool Error::checkErrors() noexcept
{
	if (s_errorMessages.size() == 0)
		return false;

	for (const auto &error : s_errorMessages)
		std::cerr << "[Registered Error] > " << error << std::endl;

	return true;
}


