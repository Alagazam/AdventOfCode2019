#pragma once
#include "../inputReader.h"
#include "../IntcodeComputer.h"
#include <sstream>
namespace Day02
{
	int64_t Solve_A()
	{
		auto	file = aoc::OpenInputFile("Day02\\Day02.txt");
		auto	input = aoc::ReadInputInt(*file);
		input[0][1] = 12;
		input[0][2] = 2;
		return intcode::execute(input[0]);
	}

	int64_t Solve_B()
	{
		auto	file = aoc::OpenInputFile("Day02\\Day02.txt");
		const auto	input = aoc::ReadInputInt(*file);
		for (int64_t noun = 0; noun != 100; ++noun)
		{
			for (int64_t verb = 0; verb != 100; ++verb)
			{
				auto program = input[0];
				program[1] = noun;
				program[2] = verb;
				auto res = intcode::execute(program);
				if (res == 19690720) return noun * 100 + verb;
			}
		}
		return 0;
	}
}