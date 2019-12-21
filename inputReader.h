#pragma once
#include <vector>
#include <istream>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <iterator>
#include <memory>

namespace aoc2018
{

	inline std::unique_ptr<std::istream> OpenInputFile(const std::string fileName)
	{
		// Open the File
		std::unique_ptr<std::istream> in = std::make_unique<std::ifstream>(fileName);

		// Check if file is valid
		if (!*in)
		{
			std::cerr << "Cannot open the File : " << fileName << std::endl;
			throw std::runtime_error("Cannot open file");
		}
		return in;
	}

	using rowContent = std::vector<std::string>;

	inline rowContent Tokenize(std::string line)
	{
		std::istringstream iss(line);
		rowContent tokens;
		copy(std::istream_iterator<std::string>(iss),
			std::istream_iterator<std::string>(),
			back_inserter(tokens));
		return tokens;
	}

	using inputContent = std::vector<rowContent>;

	inline inputContent ReadInput(std::istream& s)
	{
		std::string	line;
		inputContent	lines;
		while (std::getline(s, line))
		{
			auto tokens = Tokenize(line);
			lines.push_back(tokens);
		}
		return lines;
	}

	using inputLines = std::vector<std::string>;

	inline inputLines ReadInputLines(std::istream& s)
	{
		std::string	line;
		inputLines	lines;
		while (std::getline(s, line))
		{
			lines.push_back(line);
		}
		return lines;
	}
}