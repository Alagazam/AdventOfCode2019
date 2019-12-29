#pragma once
#include "inputReader.h"
#include <list>

namespace intcode
{
	using program_output = std::list<int64_t>;
	using program_output_ptr = std::shared_ptr<program_output>;

	enum class mode { position, immediate };

	int64_t getOpCode(int64_t val, mode& mode1, mode& mode2, mode& mode3)
	{
		auto opcode = val % 100;
		val /= 100;
		mode1 = static_cast<mode>(val % 10);
		val /= 10;
		mode2 = static_cast<mode>(val % 10);
		val /= 10;
		mode3 = static_cast<mode>(val % 10);
		return opcode;
	}
	TEST_CASE("getOpCode") {
		mode mode1;
		mode mode2;
		mode mode3;
		auto opcode = getOpCode(1002, mode1, mode2, mode3);
		CHECK(opcode == 2);
		CHECK(mode1 == mode::position);
		CHECK(mode2 == mode::immediate);
		CHECK(mode3 == mode::position);
	}

	int64_t	getValue(const aoc::rowContentInt& program, int64_t pos, mode m)
	{
		if (m == mode::position)
		{
			return program[static_cast<aoc::rowContentInt::size_type>(pos)];
		}
		else if (m == mode::immediate)
		{
			return pos;
		}
		return 0;
	}
	TEST_CASE("getValue") {
		aoc::rowContentInt	program{ 1, 2, 55, 0, 99 };
		CHECK(55 == getValue(program, 2, mode::position));
		CHECK(55 == getValue(program, 55, mode::immediate));
	}

	void setValue(aoc::rowContentInt& program, int64_t pos, int64_t val)
	{
		auto p = static_cast<aoc::rowContentInt::size_type>(pos);
		if (p < program.size())
		{
			program[p] = val;
		}
	}
	TEST_CASE("setValue") {
		aoc::rowContentInt	program{ 1, 2, 55, 0, 99 };
		setValue(program, 2, 33);
		CHECK(program == aoc::rowContentInt{ 1, 2, 33, 0, 99 });
	}

	auto	execute(aoc::rowContentInt& program, int64_t input = 0, program_output_ptr output = nullptr ) -> aoc::rowContentInt::value_type
	{
		auto	pc{ program.cbegin() };
		while (*pc != 99)
		{
			mode mode1;
			mode mode2;
			mode mode3;
			auto opcode = getOpCode(*pc, mode1, mode2, mode3);
			switch (opcode)
			{
				// Opcode 1 adds together numbers read from two positions and stores the result in a third position.
				// The three integers immediately after the opcode tell you these three positions
				// - the first two indicate the positions from which you should read the input values, 
				// and the third indicates the position at which the output should be stored.
			case 1: {
				auto val1{ getValue(program, *(pc + 1), mode1) };
				auto val2{ getValue(program, *(pc + 2), mode2) };
				auto res = val1 + val2;
				program[getValue(program, *(pc + 3), mode::immediate)] = res;
				pc += 4;
				break;
			}
				  // Opcode 2 works exactly like opcode 1, except it multiplies the two inputs instead of adding them. 
				  // Again, the three integers after the opcode indicate where the inputs and outputs are, not their values.
			case 2: {
				auto val1{ getValue(program, *(pc + 1), mode1) };
				auto val2{ getValue(program, *(pc + 2), mode2) };
				auto res = val1 * val2;
				program[getValue(program, *(pc + 3), mode::immediate)] = res;
				pc += 4;
				break;
			}
				  // Opcode 3 takes a single integer as input and saves it to the position given by its only parameter.
				  // For example, the instruction 3, 50 would take an input value and store it at address 50.
			case 3: {
				auto addr = *(pc + 1);
				program[getValue(program, addr, mode::immediate)] = input;
				pc += 2;
				break;
			}
				  // Opcode 4 outputs the value of its only parameter.
				  // For example, the instruction 4, 50 would output the value at address 50.
			case 4: {
				auto addr = *(pc + 1);
				auto val = getValue(program, addr, mode1);
				output->push_back(val);
				pc += 2;
				break;
			}

			}
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

	TEST_CASE("execute input") {
		aoc::rowContentInt	program{ 3,0,99 };

		execute(program, 55);
		CHECK(program == aoc::rowContentInt{ 55,0,99 });
	}

	TEST_CASE("execute output") {
		aoc::rowContentInt	program{ 4,2,99 };
		program_output_ptr output(new program_output);

		execute(program, 0, output);
		REQUIRE(output->size() > 0);
		CHECK(output->front() == 99);
	}

	// The program 3,0,4,0,99 outputs whatever it gets as input, then halts.
	TEST_CASE("execute input_output") {
		aoc::rowContentInt	program{ 3,0,4,0,99 };
		program_output_ptr output( new program_output );

		execute(program, 55, output);
		REQUIRE(output->size() > 0);
		CHECK(output->front() == 55);
	}

	// 1002,4,3,4,33
	TEST_CASE("execute modes") {
		aoc::rowContentInt	program{ 1002,4,3,4,33 };
		program_output_ptr output(new program_output);

		execute(program);
		CHECK(program == aoc::rowContentInt{ 1002,4,3,4,99 });
	}

	// 1101,100,-1,4,0
	TEST_CASE("execute negative") {
		aoc::rowContentInt	program{ 1101,100,-1,4,0 };
		program_output_ptr output(new program_output);

		execute(program);
		CHECK(program == aoc::rowContentInt{ 1101,100,-1,4,99 });
	}
}