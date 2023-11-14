#pragma once

#include <list>
#include <string>


class Error final
{
	public:
		static void pushErrorToStack(const std::string &errorMessage) noexcept;
		/**
		* @return true if there are errors, false otherwise
		*/
		static bool checkErrors() noexcept;

	private:
		static std::list<std::string> s_errorMessages;

		Error() = default;
		~Error() = default;
};