#include <map>

#include "converter.hpp"
#include "ops.hpp"
#include "resolver.hpp"



std::vector<uint8_t> convert(const std::vector<Instruction> &resolvedInstructions)
{
	static std::map<InstructionName, std::vector<uint8_t> (*)(const std::vector<Args> &args)> convertMaps {
		{InstructionName::NOP, [](const std::vector<Args> &args) -> std::vector<uint8_t> {
			return {(uint8_t)OpCode::NOP};
		}},
		{InstructionName::ADDS, [](const std::vector<Args> &args) -> std::vector<uint8_t> {
			return {
				(uint8_t)((int)OpCode::ADD | (int)std::get<RegisterID> (args[2].value)),
				(uint8_t)(((int)(std::get<RegisterID> (args[0].value)) << 3) | (int)std::get<RegisterID> (args[1].value))
			};
		}},
		{InstructionName::SUBS, [](const std::vector<Args> &args) -> std::vector<uint8_t> {
			return {
				(uint8_t)((int)OpCode::SUB | (int)std::get<RegisterID> (args[2].value)),
				(uint8_t)(((int)(std::get<RegisterID> (args[0].value)) << 3) | (int)std::get<RegisterID> (args[1].value))
			};
		}},
		{InstructionName::LTEQS, [](const std::vector<Args> &args) -> std::vector<uint8_t> {
			return {
				(uint8_t)((int)OpCode::LTEQ | (int)std::get<RegisterID> (args[2].value)),
				(uint8_t)(((int)(std::get<RegisterID> (args[0].value)) << 3) | (int)std::get<RegisterID> (args[1].value))
			};
		}},
		{InstructionName::GTEQS, [](const std::vector<Args> &args) -> std::vector<uint8_t> {
			return {
				(uint8_t)((int)OpCode::GTEQ | (int)std::get<RegisterID> (args[2].value)),
				(uint8_t)(((int)(std::get<RegisterID> (args[0].value)) << 3) | (int)std::get<RegisterID> (args[1].value))
			};
		}},
		{InstructionName::LTS, [](const std::vector<Args> &args) -> std::vector<uint8_t> {
			return {
				(uint8_t)((int)OpCode::LT | (int)std::get<RegisterID> (args[2].value)),
				(uint8_t)(((int)(std::get<RegisterID> (args[0].value)) << 3) | (int)std::get<RegisterID> (args[1].value))
			};
		}},
		{InstructionName::GTS, [](const std::vector<Args> &args) -> std::vector<uint8_t> {
			return {
				(uint8_t)((int)OpCode::GT | (int)std::get<RegisterID> (args[2].value)),
				(uint8_t)(((int)(std::get<RegisterID> (args[0].value)) << 3) | (int)std::get<RegisterID> (args[1].value))
			};
		}},
		{InstructionName::EQS, [](const std::vector<Args> &args) -> std::vector<uint8_t> {
			return {
				(uint8_t)((int)OpCode::EQ | (int)std::get<RegisterID> (args[2].value)),
				(uint8_t)(((int)(std::get<RegisterID> (args[0].value)) << 3) | (int)std::get<RegisterID> (args[1].value))
			};
		}},
		{InstructionName::ANDS, [](const std::vector<Args> &args) -> std::vector<uint8_t> {
			return {
				(uint8_t)((int)OpCode::AND | (int)std::get<RegisterID> (args[2].value)),
				(uint8_t)(((int)(std::get<RegisterID> (args[0].value)) << 3) | (int)std::get<RegisterID> (args[1].value))
			};
		}},
		{InstructionName::ORS, [](const std::vector<Args> &args) -> std::vector<uint8_t> {
			return {
				(uint8_t)((int)OpCode::OR | (int)std::get<RegisterID> (args[2].value)),
				(uint8_t)(((int)(std::get<RegisterID> (args[0].value)) << 3) | (int)std::get<RegisterID> (args[1].value))
			};
		}},
		{InstructionName::XORS, [](const std::vector<Args> &args) -> std::vector<uint8_t> {
			return {
				(uint8_t)((int)OpCode::XOR | (int)std::get<RegisterID> (args[2].value)),
				(uint8_t)(((int)(std::get<RegisterID> (args[0].value)) << 3) | (int)std::get<RegisterID> (args[1].value))
			};
		}},
		{InstructionName::NOTS, [](const std::vector<Args> &args) -> std::vector<uint8_t> {
			return {
				(uint8_t)((int)OpCode::NOT | (int)std::get<RegisterID> (args[1].value)),
				(uint8_t)(((int)(std::get<RegisterID> (args[0].value)) << 3))
			};
		}},
		{InstructionName::LDX, [](const std::vector<Args> &args) -> std::vector<uint8_t> {
			return {
				(uint8_t)((int)OpCode::LDX | (int)std::get<RegisterID> (args[0].value))
			};
		}},
		{InstructionName::RI, [](const std::vector<Args> &args) -> std::vector<uint8_t> {
			return {
				(uint8_t)((int)OpCode::RI | (int)std::get<RegisterID> (args[1].value)),
				(uint8_t)(((int)(std::get<RegisterID> (args[0].value)) << 3))
			};
		}},
		{InstructionName::CARRY, [](const std::vector<Args> &args) -> std::vector<uint8_t> {
			return {
				(uint8_t)((int)OpCode::CARRY | (int)std::get<RegisterID> (args[0].value))
			};
		}},
		{InstructionName::WI, [](const std::vector<Args> &args) -> std::vector<uint8_t> {
			return {
				(uint8_t)((int)OpCode::WI | (int)std::get<RegisterID> (args[1].value)),
				(uint8_t)(((int)(std::get<RegisterID> (args[0].value)) << 3))
			};
		}},
		{InstructionName::COPY, [](const std::vector<Args> &args) -> std::vector<uint8_t> {
			return {
				(uint8_t)((int)OpCode::COPY | (int)std::get<RegisterID> (args[0].value)),
				(uint8_t)(((int)(std::get<RegisterID> (args[1].value)) << 3))
			};
		}},
		{InstructionName::JHL, [](const std::vector<Args> &args) -> std::vector<uint8_t> {
			return {
				(uint8_t)OpCode::JMP
			};
		}},
		{InstructionName::JIHL, [](const std::vector<Args> &args) -> std::vector<uint8_t> {
			return {
				(uint8_t)((int)OpCode::JI | (int)std::get<RegisterID> (args[0].value))
			};
		}},
		{InstructionName::JZHL, [](const std::vector<Args> &args) -> std::vector<uint8_t> {
			return {
				(uint8_t)((int)OpCode::JZ | (int)std::get<RegisterID> (args[0].value))
			};
		}},
		{InstructionName::JCHL, [](const std::vector<Args> &args) -> std::vector<uint8_t> {
			return {
				(uint8_t)OpCode::JC
			};
		}},
		{InstructionName::MWRITE, [](const std::vector<Args> &args) -> std::vector<uint8_t> {
			return {
				(uint8_t)((int)OpCode::MWRITE | (int)std::get<RegisterID> (args[0].value))
			};
		}},
		{InstructionName::MREAD, [](const std::vector<Args> &args) -> std::vector<uint8_t> {
			return {
				(uint8_t)((int)OpCode::MREAD | (int)std::get<RegisterID> (args[0].value))
			};
		}},
		{InstructionName::BYTE, [](const std::vector<Args> &args) -> std::vector<uint8_t> {
			return {
				(uint8_t)std::get<int> (args[0].value)
			};
		}}
	};


	std::vector<uint8_t> binaryInstructions {};

	for (const auto &instruction : resolvedInstructions)
	{
		auto binaries = convertMaps[instruction.name](instruction.args);
		binaryInstructions.resize(binaryInstructions.size() + binaries.size());
		std::copy(binaries.begin(), binaries.end(), binaryInstructions.end() - binaries.size());
	}

	return binaryInstructions;
}