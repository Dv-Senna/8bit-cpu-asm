#include <bitset>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "error.hpp"
#include "converter.hpp"
#include "parser.hpp"
#include "resolver.hpp"



std::pair<std::string, std::string> handleCmdArgs(int argc, char *argv[], bool &enableOnlySyntaxeChecker);


int main(int argc, char *argv[])
{
	try
	{
		bool enableOnlySyntaxeChecker {false};
		const auto filePaths {handleCmdArgs(argc, argv, enableOnlySyntaxeChecker)};
		if (filePaths.first == "")
			return EXIT_SUCCESS;

		const std::string sourceFilePath {filePaths.first};
		std::string outputFilePath {filePaths.second};
		if (filePaths.second == "")
		{
			auto lastPeriod {sourceFilePath.find_last_of('.')};
			if (lastPeriod != std::string::npos)
			{
				outputFilePath.resize(lastPeriod);
				std::copy(sourceFilePath.begin(), sourceFilePath.begin() + lastPeriod, outputFilePath.begin());
				outputFilePath += ".bin";
			}
		}

		
		std::ifstream sourceFile {sourceFilePath};
		if (!sourceFile)
			throw std::runtime_error("Can't open file '" + sourceFilePath + "'. It may not exist");

		auto parsedInstructions {parse(sourceFile)};
		if (Error::checkErrors())
			throw std::runtime_error("Error occure during parsing");

		if (enableOnlySyntaxeChecker)
			return EXIT_SUCCESS;

		auto resolvedInstructions {resolve(parsedInstructions)};
		auto convertedBinary {convert(resolvedInstructions)};


		std::ofstream outputFile {outputFilePath, std::ios::binary};

		for (const auto &instruction : convertedBinary)
			outputFile << instruction;

		for (size_t i {0}; i < 65536 - convertedBinary.size(); ++i)
			outputFile << (uint8_t)0;

		outputFile << std::flush;
	}

	catch (const std::exception &exception)
	{
		std::cerr << "ERROR : " << exception.what() << std::endl;
		return EXIT_FAILURE;
	}


	return EXIT_SUCCESS;
}












std::pair<std::string, std::string> handleCmdArgs(int argc, char *argv[], bool &enableOnlySyntaxeChecker)
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
			std::cout << "RSASM should be use as follow (order matter !) : assembler [source_file_path] {options}. Here are the supported options :\n";
			std::cout << "\t-o [file] : set the output file\n";
			std::cout << "\t-c        : check the syntaxe validity (this will prevent RSASM to generate a binary output, and thus will overwrite -o)\n";
			std::cout << "\t--version : output the version of RSASM (this will overwrite all other options)\n";
			std::cout << "\t--help    : print this page (this will overwrite all other options)\n";
			std::cout << std::flush;
			return {"", ""};
		}

		if (arg == "--version")
		{
			std::cout << "RSASM (Really Specific Assembly) version 1.0.0" << std::endl;
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

		if (arg == "-c")
		{
			enableOnlySyntaxeChecker = true;
			return files;
		}

		if (arg == "-o")
		{
			lastCommand = "-o";
			isNextLinked = true;
		}
	}

	return files;
}
