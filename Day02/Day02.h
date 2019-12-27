#pragma once
#include "../inputReader.h"
#include <sstream>
namespace Day02
{

	auto	execute(aoc::rowContentInt& program) -> aoc::rowContentInt::value_type
	{
		auto	pc{ program.cbegin() };
		while (*pc != 99)
		{
			auto op1{ pc + 1 };
			auto val1{ program[static_cast<aoc::rowContentInt::size_type>(*op1)] };
			auto op2{ pc + 2 };
			auto val2{ program[static_cast<aoc::rowContentInt::size_type>(*op2)] };
			auto output{ *(pc + 3) };
			aoc::rowContentInt::value_type res{ 0 };
			if (*pc == 1)
			{
				res = val1 + val2;
			}
			else if (*pc == 2)
			{
				res = val1 * val2;

			}
			program[static_cast<aoc::rowContentInt::size_type>(output)] = res;
			pc += 4;
		}
		return program[0];
	}

	// 1, 0, 0, 0, 99				becomes 2, 0, 0, 0, 99 (1 + 1 = 2).
	TEST_CASE("execute ex1") {
		aoc::rowContentInt	program{ 1, 0, 0, 0, 99 };
		auto res = execute(program);
		CHECK(res == 2);
		CHECK(program == aoc::rowContentInt{ 2, 0, 0, 0, 99 });
	}
	// 2, 3, 0, 3, 99				becomes 2, 3, 0, 6, 99 (3 * 2 = 6).
	TEST_CASE("execute ex2") {
		aoc::rowContentInt	program{ 2, 3, 0, 3, 99 };
		auto res = execute(program);
		CHECK(res == 2);
		CHECK(program == aoc::rowContentInt{ 2, 3, 0, 6, 99 });
	}
	// 2, 4, 4, 5, 99, 0			becomes 2, 4, 4, 5, 99, 9801 (99 * 99 = 9801).
	TEST_CASE("execute ex3") {
		aoc::rowContentInt	program{ 2, 4, 4, 5, 99, 0 };
		auto res = execute(program);
		CHECK(res == 2);
		CHECK(program == aoc::rowContentInt{ 2, 4, 4, 5, 99, 9801 });
	}
	// 1, 1, 1, 4, 99, 5, 6, 0, 99	becomes 30, 1, 1, 4, 2, 5, 6, 0, 99.
	TEST_CASE("execute ex4") {
		aoc::rowContentInt	program{ 1, 1, 1, 4, 99, 5, 6, 0, 99 };
		auto res = execute(program);
		CHECK(res == 30);
		CHECK(program == aoc::rowContentInt{ 30, 1, 1, 4, 2, 5, 6, 0, 99 });
	}
	// 1,9,10,3,2,3,11,0,99,30,40,50		3500,9,10,70,2, 3, 11, 0,99,30, 40, 50
	TEST_CASE("execute ex5") {
		aoc::rowContentInt	program{ 1,9,10,3,2,3,11,0,99,30,40,50 };
		auto res = execute(program);
		CHECK(res == 3500);
		CHECK(program == aoc::rowContentInt{ 3500,9,10,70,2, 3, 11, 0,99,30, 40, 50 });
	}

	int64_t Solve_A()
	{
		auto	file = aoc::OpenInputFile("Day02\\Day02.txt");
		auto	input = aoc::ReadInputInt(*file);
		input[0][1] = 12;
		input[0][2] = 2;
		return execute(input[0]);
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
				auto res = execute(program);
				if (res == 19690720) return noun * 100 + verb;
			}
		}
		return 0;
	}


}