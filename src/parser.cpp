#include <map>
#include <regex>
#include <stdexcept>

#include "config.hpp"
#include "error.hpp"
#include "tag.hpp"
#include "parser.hpp"


std::map<InstructionName, std::vector<ArgsType>> instructionFormats {
	{InstructionName::NOP,    {}},
	{InstructionName::ADDS,   {ArgsType::registerID, ArgsType::registerID, ArgsType::registerID}},
	{InstructionName::SUBS,   {ArgsType::registerID, ArgsType::registerID, ArgsType::registerID}},
	{InstructionName::LTEQS,  {ArgsType::registerID, ArgsType::registerID, ArgsType::registerID}},
	{InstructionName::GTEQS,  {ArgsType::registerID, ArgsType::registerID, ArgsType::registerID}},
	{InstructionName::LTS,    {ArgsType::registerID, ArgsType::registerID, ArgsType::registerID}},
	{InstructionName::GTS,    {ArgsType::registerID, ArgsType::registerID, ArgsType::registerID}},
	{InstructionName::EQS,    {ArgsType::registerID, ArgsType::registerID, ArgsType::registerID}},
	{InstructionName::ANDS,   {ArgsType::registerID, ArgsType::registerID, ArgsType::registerID}},
	{InstructionName::ORS,    {ArgsType::registerID, ArgsType::registerID, ArgsType::registerID}},
	{InstructionName::XORS,   {ArgsType::registerID, ArgsType::registerID, ArgsType::registerID}},
	{InstructionName::NOTS,   {ArgsType::registerID, ArgsType::registerID}},
	{InstructionName::LDX,    {ArgsType::registerID, ArgsType::number}},
	{InstructionName::RI,     {ArgsType::registerID, ArgsType::registerID}},
	{InstructionName::CARRY,  {ArgsType::registerID}},
	{InstructionName::WI,     {ArgsType::registerID, ArgsType::registerID}},
	{InstructionName::COPY,   {ArgsType::registerID, ArgsType::registerID}},
	{InstructionName::JMP,    {ArgsType::tagAddress}},
	{InstructionName::JMPI,   {ArgsType::registerID, ArgsType::tagAddress}},
	{InstructionName::JMPZ,   {ArgsType::registerID, ArgsType::tagAddress}},
	{InstructionName::JMPC,   {ArgsType::tagAddress}},
	{InstructionName::MWRITE, {ArgsType::registerID}},
	{InstructionName::MREAD,  {ArgsType::registerID}},
	{InstructionName::BYTE,   {ArgsType::number}},
	{InstructionName::JMPR,   {}},
	{InstructionName::MOV,    {ArgsType::registerID, ArgsType::number}},
	{InstructionName::PUSH,   {ArgsType::registerID}},
	{InstructionName::POP,    {}},
	{InstructionName::CALL,   {ArgsType::tagAddress}},
	{InstructionName::RET,    {}},
	{InstructionName::ADD,    {ArgsType::registerID, ArgsType::registerID}},
	{InstructionName::SUB,    {ArgsType::registerID, ArgsType::registerID}},
	{InstructionName::LTEQ,   {ArgsType::registerID, ArgsType::registerID}},
	{InstructionName::GTEQ,   {ArgsType::registerID, ArgsType::registerID}},
	{InstructionName::LT,     {ArgsType::registerID, ArgsType::registerID}},
	{InstructionName::GT,     {ArgsType::registerID, ArgsType::registerID}},
	{InstructionName::EQ,     {ArgsType::registerID, ArgsType::registerID}},
	{InstructionName::AND,    {ArgsType::registerID, ArgsType::registerID}},
	{InstructionName::OR,     {ArgsType::registerID, ArgsType::registerID}},
	{InstructionName::XOR,    {ArgsType::registerID, ArgsType::registerID}},
	{InstructionName::NOT,    {ArgsType::registerID}},
};


bool isInstructionValid(const Instruction &instruction)
{
	auto it {instructionFormats.find(instruction.name)};
	if (it == instructionFormats.end())
		return false;

	if (instruction.args.size() != it->second.size())
		return false;

	bool breaked {false};
	for (size_t i {0}; i < it->second.size(); ++i)
	{
		if (instruction.args[i].type != it->second[i])
		{
			breaked = true;
			break;
		}
	}

	return !breaked;
}



bool isInstructionFondamental(const Instruction &instruction)
{
	return static_cast<int> (instruction.name) - static_cast<int> (InstructionName::__fondamentInstructionCount) < 0;
}



std::string interpretLine(const std::string &givenLine, std::vector<std::string> &args)
{
	args.clear();

	// remove comments
	size_t commentPosition {givenLine.find(config::commentSymbol)};
	std::string line {}, instruction {""};
	if (commentPosition == std::string::npos)
		commentPosition = givenLine.size();
	line.resize(commentPosition);
	std::copy(givenLine.begin(), givenLine.begin() + commentPosition, line.begin());


	const std::regex regex {R"(\w+)"};
	bool first {true};
	std::smatch matchs {};

	while (std::regex_search(line, matchs, regex))
	{
		if (first)
		{
			first = false;
			instruction = matchs[0];
		}

		else args.push_back(matchs[0]);

		line = matchs.suffix();
	}

	return instruction;
}



std::vector<Args> interpretStringArgs(const std::vector<std::string> &stringArgs)
{
	std::vector<Args> args {};
	args.reserve(stringArgs.size());
	for (const auto &arg : stringArgs)
	{
		char *isNumber {};
		long int number {strtol(arg.c_str(), &isNumber, 0)};
		if (*isNumber == '\0')
		{
			args.push_back({ArgsType::number, static_cast<int> (number)});
			continue;
		}

		auto registerID {getRegisterIDFromName(arg)};
		if (registerID != RegisterID::invalid)
		{
			args.push_back({ArgsType::registerID, registerID});
			continue;
		}

		args.push_back({ArgsType::tag, arg});
	}

	return args;
}



InstructionName getInstructionNameFromString(std::string name)
{
	std::map<std::string, InstructionName> instructions {
		{"nop", InstructionName::NOP},
		{"add", InstructionName::ADD},
		{"sub", InstructionName::SUB},
		{"lteq", InstructionName::LTEQ},
		{"gteq", InstructionName::GTEQ},
		{"lt", InstructionName::LT},
		{"gt", InstructionName::GT},
		{"eq", InstructionName::EQ},
		{"and", InstructionName::AND},
		{"or", InstructionName::OR},
		{"xor", InstructionName::XOR},
		{"not", InstructionName::NOT},
		{"ldx", InstructionName::LDX},
		{"ri", InstructionName::RI},
		{"carry", InstructionName::CARRY},
		{"wi", InstructionName::WI},
		{"copy", InstructionName::COPY},
		{"jmp", InstructionName::JMP},
		{"jmpi", InstructionName::JMPI},
		{"jmpz", InstructionName::JMPZ},
		{"jmpc", InstructionName::JMPC},
		{"mwrite", InstructionName::MWRITE},
		{"mread", InstructionName::MREAD},
		{"byte", InstructionName::BYTE},
		{"jmpr", InstructionName::JMPR},
		{"mov", InstructionName::MOV},
		{"push", InstructionName::PUSH},
		{"pop", InstructionName::POP},
		{"call", InstructionName::CALL},
		{"ret", InstructionName::RET}
	};

	std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) {return std::tolower(c);});

	auto it {instructions.find(name)};
	if (it == instructions.end())
		return InstructionName::invalid;

	return it->second;
}



std::vector<Instruction> parse(std::istream &stream)
{
	std::vector<Instruction> parsedInstructions {};
	std::string line {};
	int lineCount {0}, currentAddress {0};

	while (std::getline(stream, line))
	{
		++lineCount;

		if (line == "")
			continue;

		std::vector<std::string> stringArgs {};
		std::string instruction {interpretLine(line, stringArgs)};

		if (instruction == "")
			continue;

		auto args {interpretStringArgs(stringArgs)};
		auto instructionName {getInstructionNameFromString(instruction)};
		if (instructionName == InstructionName::invalid)
		{
			size_t wordPosition {line.find(instruction)};
			if (wordPosition != std::string::npos)
			{
				size_t doubleDotPosition {wordPosition + instruction.size()};
				if (doubleDotPosition < line.size())
				{
					if (line[doubleDotPosition] == ':')
					{
						if (args.size() != 0)
						{
							Error::pushErrorToStack("Line " + std::to_string(lineCount) + " : invalid syntax after tag creation");
							continue;
						}

						if (Tag::doTagExist(instruction))
						{
							Error::pushErrorToStack("Line " + std::to_string(lineCount) + " : tag '" + instruction + "' already exist");
							continue;
						}

						Tag::registerTag(instruction, currentAddress);
						continue;
					}
				}
			}

			Error::pushErrorToStack("Line " + std::to_string(lineCount) + " : instruction name '" + instruction + "' is invalid");
			continue;
		}


		++currentAddress;


		parsedInstructions.push_back({
			instructionName,
			args,
			lineCount
		});
	}


	for (auto &instruction : parsedInstructions)
	{
		for (auto &arg : instruction.args)
		{
			if (arg.type != ArgsType::tag)
				continue;

			int address {Tag::getAddressFromName(std::get<std::string> (arg.value))};
			if (address < 0)
			{
				Error::pushErrorToStack("Line " + std::to_string(instruction.line) + " : unknown tag '" + std::get<std::string> (arg.value) + "'");
				continue;
			}

			arg.type = ArgsType::tagAddress;
			arg.value = address;
		}

		if (!isInstructionValid(instruction))
			Error::pushErrorToStack("Line " + std::to_string(instruction.line) + " : invalid syntaxe");
	}


	return parsedInstructions;
}


