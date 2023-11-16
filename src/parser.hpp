#pragma once

#include <iostream>
#include <string>
#include <variant>
#include <vector>

#include "register.hpp"


enum class InstructionName
{
	// fondamentals instructions
	NOP = 0,
	ADDS,
	SUBS,
	LTEQS,
	GTEQS,
	LTS,
	GTS,
	EQS,
	ANDS,
	ORS,
	XORS,
	NOTS,
	LDX,
	RI,
	CARRY,
	WI,
	COPY,
	JHL,
	JIHL,
	JZHL,
	JCHL,
	MWRITE,
	MREAD,
	BYTE,

	__fondamentInstructionCount,

	// start of composed instructions
	MOV,
	PUSH,
	POP,
	CALL,
	RET,
	ADD,
	SUB,
	LTEQ,
	GTEQ,
	LT,
	GT,
	EQ,
	AND,
	OR,
	XOR,
	NOT,
	JMP,
	JI,
	JZ,
	JC,

	invalid
};


enum class ArgsType
{
	tag,
	number,
	registerID,
	tagAddress
};


struct Args
{
	ArgsType type;
	std::variant<std::string, int, RegisterID> value;
};


struct Instruction
{
	InstructionName name;
	std::vector<Args> args;
	int line {};
};


bool isInstructionValid(const Instruction &instruction);
bool isInstructionFondamental(const Instruction &instruction);
std::vector<Instruction> parse(std::istream &stream);