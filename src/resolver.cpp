#include <cstdint>
#include <fstream>
#include <map>

#include "resolver.hpp"



bool isBigEndian()
{
	union {
		uint32_t i;
		char c[4];
	} bint = {0x01020304};

	return bint.c[0] == 1;
}



std::vector<Instruction> resolve(const std::vector<Instruction> &parsedInstructions)
{
	static bool bigEndianness {isBigEndian()};
	static std::map<InstructionName, std::vector<Instruction> (*)(const std::vector<Args> &args)> instructionDecompositions {
		{InstructionName::MOV,  [](const std::vector<Args> &args) -> std::vector<Instruction> {return {
				{InstructionName::LDX, {
					{ArgsType::registerID, args[0].value}
				}},
				{InstructionName::BYTE, {
					{ArgsType::number, args[1].value}
				}}
			};
		}},
		{InstructionName::PUSH, [](const std::vector<Args> &args) -> std::vector<Instruction> {return {
				{InstructionName::LDX, {
					{ArgsType::registerID, RegisterID::H}
				}},
				{InstructionName::BYTE, {
					{ArgsType::number, 0}
				}},
				{InstructionName::COPY, {
					{ArgsType::registerID, RegisterID::L},
					{ArgsType::registerID, RegisterID::F}
				}},
				{InstructionName::MWRITE, {
					{ArgsType::registerID, args[0].value}
				}},
				{InstructionName::LDX, {
					{ArgsType::registerID, RegisterID::E}
				}},
				{InstructionName::BYTE, {
					{ArgsType::number, 2}
				}},
				{InstructionName::ADDS, {
					{ArgsType::registerID, RegisterID::F},
					{ArgsType::registerID, RegisterID::E},
					{ArgsType::registerID, RegisterID::F}
				}},
			};
		}},
		{InstructionName::POP,  [](const std::vector<Args> &args) -> std::vector<Instruction> {return {
				{InstructionName::LDX, {
					{ArgsType::registerID, RegisterID::E}
				}},
				{InstructionName::BYTE, {
					{ArgsType::number, 2}
				}},
				{InstructionName::SUBS, {
					{ArgsType::registerID, RegisterID::F},
					{ArgsType::registerID, RegisterID::E},
					{ArgsType::registerID, RegisterID::F}
				}}
			};
		}},
		{InstructionName::CALL, [](const std::vector<Args> &args) -> std::vector<Instruction> {
			uint8_t high {}, low {}, rethigh {}, retlow {};
			uint16_t address {static_cast<uint16_t> (std::get<int> (args[0].value))};
			uint16_t retaddress {static_cast<uint16_t> (std::get<int> (args[1].value))};
			high = bigEndianness ? ((uint8_t*)(&address))[0] : ((uint8_t*)(&address))[1];
			low  = bigEndianness ? ((uint8_t*)(&address))[1] : ((uint8_t*)(&address))[0];
			rethigh = bigEndianness ? ((uint8_t*)(&retaddress))[0] : ((uint8_t*)(&retaddress))[1];
			retlow  = bigEndianness ? ((uint8_t*)(&retaddress))[1] : ((uint8_t*)(&retaddress))[0];
			return {
				{InstructionName::LDX, {
					{ArgsType::registerID, RegisterID::A}
				}},
				{InstructionName::BYTE, {
					{ArgsType::number, (int)rethigh}
				}},
				{InstructionName::PUSH, {
					{ArgsType::registerID, RegisterID::A}
				}},
				{InstructionName::LDX, {
					{ArgsType::registerID, RegisterID::A}
				}},
				{InstructionName::BYTE, {
					{ArgsType::number, (int)retlow}
				}},
				{InstructionName::LDX, {
					{ArgsType::registerID, RegisterID::E}
				}},
				{InstructionName::BYTE, {
					{ArgsType::number, 1}
				}},
				{InstructionName::ADDS, {
					{ArgsType::registerID, RegisterID::L},
					{ArgsType::registerID, RegisterID::E},
					{ArgsType::registerID, RegisterID::L}
				}},
				{InstructionName::MWRITE, {
					{ArgsType::registerID, RegisterID::A}
				}},
				{InstructionName::LDX, {
					{ArgsType::registerID, RegisterID::H}
				}},
				{InstructionName::BYTE, {
					{ArgsType::number, (int)high}
				}},
				{InstructionName::JMP, {
					{ArgsType::tagAddress, args[0].value}
				}}
			};
		}},
		{InstructionName::RET,  [](const std::vector<Args> &args) -> std::vector<Instruction> {return {
				{InstructionName::POP, {}},
				{InstructionName::LDX, {
					{ArgsType::registerID, RegisterID::H}
				}},
				{InstructionName::BYTE, {
					{ArgsType::number, 0}
				}},
				{InstructionName::COPY, {
					{ArgsType::registerID, RegisterID::L},
					{ArgsType::registerID, RegisterID::F}
				}},
				{InstructionName::MREAD, {
					{ArgsType::registerID, RegisterID::D}
				}},
				{InstructionName::LDX, {
					{ArgsType::registerID, RegisterID::E}
				}},
				{InstructionName::BYTE, {
					{ArgsType::number, 1}
				}},
				{InstructionName::ADDS, {
					{ArgsType::registerID, RegisterID::L},
					{ArgsType::registerID, RegisterID::E},
					{ArgsType::registerID, RegisterID::L}
				}},
				{InstructionName::MREAD, {
					{ArgsType::registerID, RegisterID::E}
				}},
				{InstructionName::COPY, {
					{ArgsType::registerID, RegisterID::H},
					{ArgsType::registerID, RegisterID::D}
				}},
				{InstructionName::COPY, {
					{ArgsType::registerID, RegisterID::L},
					{ArgsType::registerID, RegisterID::E}
				}},
				{InstructionName::JHL, {}}
			};
		}},
		{InstructionName::ADD,  [](const std::vector<Args> &args) -> std::vector<Instruction> {return {
			{InstructionName::ADDS, {
				{ArgsType::registerID, args[0].value},
				{ArgsType::registerID, args[1].value},
				{ArgsType::registerID, args[0].value}
			}}};
		}},
		{InstructionName::SUB,  [](const std::vector<Args> &args) -> std::vector<Instruction> {return {
			{InstructionName::SUBS, {
				{ArgsType::registerID, args[0].value},
				{ArgsType::registerID, args[1].value},
				{ArgsType::registerID, args[0].value}
			}}};
		}},
		{InstructionName::LTEQ, [](const std::vector<Args> &args) -> std::vector<Instruction> {return {
			{InstructionName::LTEQS, {
				{ArgsType::registerID, args[0].value},
				{ArgsType::registerID, args[1].value},
				{ArgsType::registerID, args[0].value}
			}}};
		}},
		{InstructionName::GTEQ, [](const std::vector<Args> &args) -> std::vector<Instruction> {return {
			{InstructionName::GTEQS, {
				{ArgsType::registerID, args[0].value},
				{ArgsType::registerID, args[1].value},
				{ArgsType::registerID, args[0].value}
			}}};
		}},
		{InstructionName::LT,   [](const std::vector<Args> &args) -> std::vector<Instruction> {return {
			{InstructionName::LTS, {
				{ArgsType::registerID, args[0].value},
				{ArgsType::registerID, args[1].value},
				{ArgsType::registerID, args[0].value}
			}}};
		}},
		{InstructionName::GT,   [](const std::vector<Args> &args) -> std::vector<Instruction> {return {
			{InstructionName::GTS, {
				{ArgsType::registerID, args[0].value},
				{ArgsType::registerID, args[1].value},
				{ArgsType::registerID, args[0].value}
			}}};
		}},
		{InstructionName::EQ,   [](const std::vector<Args> &args) -> std::vector<Instruction> {return {
			{InstructionName::EQS, {
				{ArgsType::registerID, args[0].value},
				{ArgsType::registerID, args[1].value},
				{ArgsType::registerID, args[0].value}
			}}};
		}},
		{InstructionName::AND,  [](const std::vector<Args> &args) -> std::vector<Instruction> {return {
			{InstructionName::ANDS, {
				{ArgsType::registerID, args[0].value},
				{ArgsType::registerID, args[1].value},
				{ArgsType::registerID, args[0].value}
			}}};
		}},
		{InstructionName::OR,   [](const std::vector<Args> &args) -> std::vector<Instruction> {return {
			{InstructionName::ORS, {
				{ArgsType::registerID, args[0].value},
				{ArgsType::registerID, args[1].value},
				{ArgsType::registerID, args[0].value}
			}}};
		}},
		{InstructionName::XOR,  [](const std::vector<Args> &args) -> std::vector<Instruction> {return {
			{InstructionName::XORS, {
				{ArgsType::registerID, args[0].value},
				{ArgsType::registerID, args[1].value},
				{ArgsType::registerID, args[0].value}
			}}};
		}},
		{InstructionName::NOT,  [](const std::vector<Args> &args) -> std::vector<Instruction> {return {
			{InstructionName::NOTS, {
				{ArgsType::registerID, args[0].value},
				{ArgsType::registerID, args[0].value}
			}}};
		}},
		{InstructionName::JMP,  [](const std::vector<Args> &args) -> std::vector<Instruction> {
			uint16_t address {static_cast<uint16_t> (std::get<int> (args[0].value))};
			int high = bigEndianness ? ((uint8_t*)(&address))[0] : ((uint8_t*)(&address))[1];
			int low  = bigEndianness ? ((uint8_t*)(&address))[1] : ((uint8_t*)(&address))[0];
			return {
				{InstructionName::LDX, {
					{ArgsType::registerID, RegisterID::H}
				}},
				{InstructionName::BYTE, {
					{ArgsType::number, high}
				}},
				{InstructionName::LDX, {
					{ArgsType::registerID, RegisterID::L}
				}},
				{InstructionName::BYTE, {
					{ArgsType::number, low}
				}},
				{InstructionName::JHL, {}}};
		}},
		{InstructionName::JI,  [](const std::vector<Args> &args) -> std::vector<Instruction> {
			uint16_t address {static_cast<uint16_t> (std::get<int> (args[1].value))};
			int high = bigEndianness ? ((uint8_t*)(&address))[0] : ((uint8_t*)(&address))[1];
			int low  = bigEndianness ? ((uint8_t*)(&address))[1] : ((uint8_t*)(&address))[0];
			return {
				{InstructionName::LDX, {
					{ArgsType::registerID, RegisterID::H}
				}},
				{InstructionName::BYTE, {
					{ArgsType::number, high}
				}},
				{InstructionName::LDX, {
					{ArgsType::registerID, RegisterID::L}
				}},
				{InstructionName::BYTE, {
					{ArgsType::number, low}
				}},
				{InstructionName::JIHL, {
					{ArgsType::registerID, args[0].value}
				}}};
		}},
		{InstructionName::JZ,  [](const std::vector<Args> &args) -> std::vector<Instruction> {
			uint16_t address {static_cast<uint16_t> (std::get<int> (args[1].value))};
			int high = bigEndianness ? ((uint8_t*)(&address))[0] : ((uint8_t*)(&address))[1];
			int low  = bigEndianness ? ((uint8_t*)(&address))[1] : ((uint8_t*)(&address))[0];
			return {
				{InstructionName::LDX, {
					{ArgsType::registerID, RegisterID::H}
				}},
				{InstructionName::BYTE, {
					{ArgsType::number, high}
				}},
				{InstructionName::LDX, {
					{ArgsType::registerID, RegisterID::L}
				}},
				{InstructionName::BYTE, {
					{ArgsType::number, low}
				}},
				{InstructionName::JZHL, {
					{ArgsType::registerID, args[0].value}
				}}};
		}},
		{InstructionName::JC,  [](const std::vector<Args> &args) -> std::vector<Instruction> {
			uint16_t address {static_cast<uint16_t> (std::get<int> (args[0].value))};
			int high = bigEndianness ? ((uint8_t*)(&address))[0] : ((uint8_t*)(&address))[1];
			int low  = bigEndianness ? ((uint8_t*)(&address))[1] : ((uint8_t*)(&address))[0];
			return {
				{InstructionName::LDX, {
					{ArgsType::registerID, RegisterID::H}
				}},
				{InstructionName::BYTE, {
					{ArgsType::number, high}
				}},
				{InstructionName::LDX, {
					{ArgsType::registerID, RegisterID::L}
				}},
				{InstructionName::BYTE, {
					{ArgsType::number, low}
				}},
				{InstructionName::JC, {}}};
		}},
	};
	static std::ofstream output {"out.log"};

	std::vector<Instruction> lastInstructions {parsedInstructions};

	for (size_t i {0}; i < 1000; ++i)
	{
		bool isOneNotFondamental {false};

		for (const auto &instruction : lastInstructions)
		{
			if (!isInstructionFondamental(instruction))
			{
				isOneNotFondamental = true;
				break;
			}
		}

		if (!isOneNotFondamental)
			return lastInstructions;

		std::vector<Instruction> resolvedInstructions {};
		resolvedInstructions.reserve(lastInstructions.size());

		for (const auto &instruction : lastInstructions)
		{
			if (isInstructionFondamental(instruction))
			{
				resolvedInstructions.push_back(instruction);
				continue;
			}

			auto decompositions {instructionDecompositions[instruction.name](instruction.args)};
			for (const auto &decomposition : decompositions)
				resolvedInstructions.push_back(decomposition);
		}

		lastInstructions = resolvedInstructions;



		for (const auto &instruction : resolvedInstructions)
		{
			output << "Line " << instruction.line << " > " << static_cast<int> (instruction.name) << " : ";
			for (const auto &arg : instruction.args)
			{
				switch (arg.type)
				{
					case ArgsType::number:
						output << "(number) " << std::get<int> (arg.value) << ", ";
						break;

					case ArgsType::registerID:
						output << "(registerID) " << (int)std::get<RegisterID> (arg.value) << ", ";
						break;

					case ArgsType::tag:
						output << "(tag) " << std::get<std::string> (arg.value) << ", ";
						break;

					case ArgsType::tagAddress:
						output << "(tagAddress) " << std::get<int> (arg.value) << ", ";
						break;
				}
			}

			output << "\n";
		}

		output << "---------------------" << std::endl;
	}

	throw std::runtime_error("Can't resolve code : the resolver exceed the max iteration count (1000)");

	return lastInstructions;
}