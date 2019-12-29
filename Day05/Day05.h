#pragma once
#include "../inputReader.h"
#include "../IntcodeComputer.h"
#include <sstream>
namespace Day05
{
	int64_t Solve_A()
	{
		auto	file = aoc::OpenInputFile("Day05\\Day05.txt");
		auto	input = aoc::ReadInputInt(*file);

		intcode::program_output_ptr output(new intcode::program_output);
		intcode::execute(input[0], 1, output);
		return output->back();
	}

	int64_t Solve_B()
	{
		auto	file = aoc::OpenInputFile("Day05\\Day05.txt");
		const auto	input = aoc::ReadInputInt(*file);

		return 0;
	}
}