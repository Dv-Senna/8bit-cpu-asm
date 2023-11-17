#pragma once

#include <string>


enum class RegisterID
{
	A = 0,
	B = 1,
	C = 2,
	D = 3,
	E = 4,
	F = 5,
	H = 6,
	L = 7,

	invalid
};


RegisterID getRegisterIDFromName(std::string name);