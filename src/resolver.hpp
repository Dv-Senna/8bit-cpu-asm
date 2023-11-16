#pragma once


#include "parser.hpp"


bool isBigEndian();
std::vector<Instruction> resolve(const std::vector<Instruction> &parsedInstructions);