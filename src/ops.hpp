#pragma once


enum class OpCode
{
	NOP    = 0b00000000,
	ADD    = 0b10000000,
	SUB    = 0b10001000,
	LTEQ   = 0b10010000,
	GTEQ   = 0b10011000,
	LT     = 0b10100000,
	GT     = 0b10101000,
	EQ     = 0b10110000,
	AND    = 0b10111000,
	OR     = 0b11000000,
	XOR    = 0b11001000,
	NOT    = 0b11010000,
	LDX    = 0b11011000,
	RI     = 0b11100000,
	CARRY  = 0b11101000,
	WI     = 0b11110000,
	COPY   = 0b11111000,
	JMP    = 0b00100000,
	JI     = 0b00101000,
	JZ     = 0b00110000,
	JC     = 0b00111000,
	MWRITE = 0b01000000,
	MREAD  = 0b01001000,

	invalid = 0xff
};