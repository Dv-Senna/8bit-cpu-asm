#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "error.hpp"
#include "parser.hpp"
#include "resolver.hpp"



std::pair<std::string, std::string> handleCmdArgs(int argc, char *argv[]);


int main(int argc, char *argv[])
{
	try
	{
		const auto filePaths {handleCmdArgs(argc, argv)};
		if (filePaths.first == "")
			return EXIT_SUCCESS;

		const std::string sourceFilePath {filePaths.first};
		std::string outputFilePath {sourceFilePath};
		if (filePaths.second != "")
			outputFilePath = filePaths.second;

		
		std::ifstream sourceFile {sourceFilePath};
		if (!sourceFile)
			throw std::runtime_error("Can't open file '" + sourceFilePath + "'. It may not exist");

		auto parsedInstructions {parse(sourceFile)};
		if (Error::checkErrors())
			throw std::runtime_error("Error occure during parsing");

		/*for (const auto &instruction : parsedInstructions)
		{
			std::cout << "Line " << instruction.line << " > " << static_cast<int> (instruction.name) << " : ";
			for (const auto &arg : instruction.args)
			{
				switch (arg.type)
				{
					case ArgsType::number:
						std::cout << "(number) " << std::get<int> (arg.value) << ", ";
						break;

					case ArgsType::registerID:
						std::cout << "(registerID) " << (int)std::get<RegisterID> (arg.value) << ", ";
						break;

					case ArgsType::tag:
						std::cout << "(tag) " << std::get<std::string> (arg.value) << ", ";
						break;

					case ArgsType::tagAddress:
						std::cout << "(tagAddress) " << std::get<int> (arg.value) << ", ";
						break;
				}
			}

			std::cout << "\n";
		}

		std::cout << std::flush;*/

		auto resolvedInstructions {resolve(parsedInstructions)};

		for (const auto &instruction : resolvedInstructions)
		{
			std::cout << "Line " << instruction.line << " > " << static_cast<int> (instruction.name) << " : ";
			for (const auto &arg : instruction.args)
			{
				switch (arg.type)
				{
					case ArgsType::number:
						std::cout << "(number) " << std::get<int> (arg.value) << ", ";
						break;

					case ArgsType::registerID:
						std::cout << "(registerID) " << (int)std::get<RegisterID> (arg.value) << ", ";
						break;

					case ArgsType::tag:
						std::cout << "(tag) " << std::get<std::string> (arg.value) << ", ";
						break;

					case ArgsType::tagAddress:
						std::cout << "(tagAddress) " << std::get<int> (arg.value) << ", ";
						break;
				}
			}

			std::cout << "\n";
		}

		std::cout << std::flush;
	}

	catch (const std::exception &exception)
	{
		std::cerr << "ERROR : " << exception.what() << std::endl;
		return EXIT_FAILURE;
	}


	return EXIT_SUCCESS;
}












std::pair<std::string, std::string> handleCmdArgs(int argc, char *argv[])
{
	if (argc <= 1)
		throw std::runtime_error("Please specify a file to assemble, or use --help");

	std::vector<std::string> args {};
	args.reserve(argc - 1);
	for (int i {1}; i < argc; ++i)
		args.push_back(std::string(argv[i]));

	std::pair<std::string, std::string> files {"", ""};
	bool firstArg {true};
	bool isNextLinked {false};
	std::string lastCommand {""};

	for (const auto &arg : args)
	{
		if (arg == "--help")
		{
			std::cout << "This assembly should be use as follow (order matter !) : assembler [source_file_path] {options}. Here are the supported options :\n";
			std::cout << "\t-o [file] : set the output file\n";
			std::cout << "\t--help    : print this page (this will overwrite all other options)\n";
			std::cout << std::flush;
			return {"", ""};
		}

		if (arg.size() != 0 && (firstArg || isNextLinked) && arg[0] == '-')
			throw std::runtime_error("First argument should be the source file");

		if (firstArg)
		{
			firstArg = false;
			files.first = arg;
			continue;
		}

		if (isNextLinked)
		{
			isNextLinked = false;

			if (lastCommand == "-o")
				files.second = arg;

			continue;
		}

		if (arg == "-o")
		{
			lastCommand = "-o";
			isNextLinked = true;
		}
	}

	return files;
}
