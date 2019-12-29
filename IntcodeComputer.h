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
				setValue(program, *(pc + 3), res);
				pc += 4;
				break;
			}
				  // Opcode 2 works exactly like opcode 1, except it multiplies the two inputs instead of adding them. 
				  // Again, the three integers after the opcode indicate where the inputs and outputs are, not their values.
			case 2: {
				auto val1{ getValue(program, *(pc + 1), mode1) };
				auto val2{ getValue(program, *(pc + 2), mode2) };
				auto res = val1 * val2;
				setValue(program, *(pc + 3), res);
				pc += 4;
				break;
			}
				  // Opcode 3 takes a single integer as input and saves it to the position given by its only parameter.
				  // For example, the instruction 3, 50 would take an input value and store it at address 50.
			case 3: {
				auto addr = *(pc + 1);
				setValue(program, addr, input);
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

				// Opcode 5 is jump-if-true: if the first parameter is non-zero, 
				// it sets the instruction pointer to the value from the second parameter.Otherwise, it does nothing.
			case 5: {
				auto addr = *(pc + 1);
				auto val = getValue(program, addr, mode1);
				if (val != 0)
				{
					pc = begin(program) + getValue(program, *(pc + 2), mode2);
				}
				else
				{
					pc += 3;
				}
				break;
			}
				  
				// Opcode 6 is jump-if-false: if the first parameter is zero, 
				// it sets the instruction pointer to the value from the second parameter.Otherwise, it does nothing.
			case 6: {
				auto addr = *(pc + 1);
				auto val = getValue(program, addr, mode1);
				if (val == 0)
				{
					pc = begin(program) + getValue(program, *(pc + 2), mode2);
				}
				else
				{
					pc += 3;
				}
				break;
			}
				// Opcode 7 is less than: if the first parameter is less than the second parameter,
				// it stores 1 in the position given by the third parameter.Otherwise, it stores 0.
			case 7: {
				auto val1 = getValue(program, *(pc + 1), mode1);
				auto val2 = getValue(program, *(pc + 2), mode2);
				setValue(program, *(pc + 3), (val1 < val2) ? 1 : 0);
				pc += 4;
				break;
			}

				// Opcode 8 is equals: if the first parameter is equal to the second parameter,
				//it stores 1 in the position given by the third parameter.Otherwise, it stores 0.
			case 8: {
				auto val1 = getValue(program, *(pc + 1), mode1);
				auto val2 = getValue(program, *(pc + 2), mode2);
				setValue(program, *(pc + 3), (val1 == val2) ? 1 : 0);
				pc += 4;
				break;
			}

			default:
				return -1;
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


	//3,9,8,9,10,9,4,9,99,-1,8 - Using position mode, consider whether the input is equal to 8; output 1 (if it is) or 0 (if it is not).
	TEST_CASE("equal 8 position") {
		aoc::rowContentInt	program{ 3,9,8,9,10,9,4,9,99,-1,8 };
		program_output_ptr output(new program_output);
		execute(program, 8, output);
		REQUIRE(output->size() > 0);
		CHECK(output->front() == 1);
	}
	TEST_CASE("not equal 8 position") {
		aoc::rowContentInt	program{ 3,9,8,9,10,9,4,9,99,-1,8 };
		program_output_ptr output(new program_output);
		execute(program, 5, output);
		REQUIRE(output->size() > 0);
		CHECK(output->front() == 0);
	}
	//3,9,7,9,10,9,4,9,99,-1,8 - Using position mode, consider whether the input is less than 8; output 1 (if it is) or 0 (if it is not).
	TEST_CASE("less than 8 position") {
		aoc::rowContentInt	program{ 3,9,7,9,10,9,4,9,99,-1,8 };
		program_output_ptr output(new program_output);
		execute(program, 7, output);
		REQUIRE(output->size() > 0);
		CHECK(output->front() == 1);
	}
	TEST_CASE("not less than 8 position") {
		aoc::rowContentInt	program{ 3,9,7,9,10,9,4,9,99,-1,8 };
		program_output_ptr output(new program_output);
		execute(program, 8, output);
		REQUIRE(output->size() > 0);
		CHECK(output->front() == 0);
	}
	//3,3,1108,-1,8,3,4,3,99 - Using immediate mode, consider whether the input is equal to 8; output 1 (if it is) or 0 (if it is not).
	TEST_CASE("equal 8 immediate") {
		aoc::rowContentInt	program{ 3,3,1108,-1,8,3,4,3,99 };
		program_output_ptr output(new program_output);
		execute(program, 8, output);
		REQUIRE(output->size() > 0);
		CHECK(output->front() == 1);
	}
	TEST_CASE("not equal 8 immediate") {
		aoc::rowContentInt	program{ 3,3,1108,-1,8,3,4,3,99 };
		program_output_ptr output(new program_output);
		execute(program, 5, output);
		REQUIRE(output->size() > 0);
		CHECK(output->front() == 0);
	}
	//3,3,1107,-1,8,3,4,3,99 - Using immediate mode, consider whether the input is less than 8; output 1 (if it is) or 0 (if it is not).
	TEST_CASE("less than 8 immediate") {
		aoc::rowContentInt	program{ 3,3,1107,-1,8,3,4,3,99 };
		program_output_ptr output(new program_output);
		execute(program, 7, output);
		REQUIRE(output->size() > 0);
		CHECK(output->front() == 1);
	}
	TEST_CASE("not less than 8 immediate") {
		aoc::rowContentInt	program{ 3,3,1107,-1,8,3,4,3,99 };
		program_output_ptr output(new program_output);
		execute(program, 8, output);
		REQUIRE(output->size() > 0);
		CHECK(output->front() == 0);
	}

	//Here are some jump tests that take an input, then output 0 if the input was zero or 1 if the input was non-zero:
	//3,12,6,12,15,1,13,14,13,4,13,99,-1,0,1,9 (using position mode)
	TEST_CASE("jump test position zero") {
		aoc::rowContentInt	program{ 3,12,6,12,15,1,13,14,13,4,13,99,-1,0,1,9 };
		program_output_ptr output(new program_output);
		execute(program, 0, output);
		REQUIRE(output->size() > 0);
		CHECK(output->front() == 0);
	}
	TEST_CASE("jump test position zero") {
		aoc::rowContentInt	program{ 3,12,6,12,15,1,13,14,13,4,13,99,-1,0,1,9 };
		program_output_ptr output(new program_output);
		execute(program, 5, output);
		REQUIRE(output->size() > 0);
		CHECK(output->front() == 1);
	}
	//3,3,1105,-1,9,1101,0,0,12,4,12,99,1 (using immediate mode)
	TEST_CASE("jump test immediate zero") {
		aoc::rowContentInt	program{ 3,12,6,12,15,1,13,14,13,4,13,99,-1,0,1,9 };
		program_output_ptr output(new program_output);
		execute(program, 0, output);
		REQUIRE(output->size() > 0);
		CHECK(output->front() == 0);
	}
	TEST_CASE("jump test immediate zero") {
		aoc::rowContentInt	program{ 3,12,6,12,15,1,13,14,13,4,13,99,-1,0,1,9 };
		program_output_ptr output(new program_output);
		execute(program, 5, output);
		REQUIRE(output->size() > 0);
		CHECK(output->front() == 1);
	}

	aoc::rowContentInt	bigTestCompre{ 3,21,1008,21,8,20,1005,20,22,107,8,21,20,1006,20,31,
	1106,0,36,98,0,0,1002,21,125,20,4,20,1105,1,46,104,
	999,1105,1,46,1101,1000,1,20,4,20,1105,1,46,98,99 };
	//The above example program uses an input instruction to ask for a single number. 
	//The program will then output 999 if the input value is below 8, 
	//output 1000 if the input value is equal to 8, 
	//or output 1001 if the input value is greater than 8.
	TEST_CASE("big test") {
		aoc::rowContentInt	program = bigTestCompre;
		program_output_ptr output(new program_output);
		execute(program, 5, output);
		REQUIRE(output->size() > 0);
		CHECK(output->front() == 999);

		program = bigTestCompre;
		output->clear();
		execute(program, 8, output);
		REQUIRE(output->size() > 0);
		CHECK(output->front() == 1000);

		program = bigTestCompre;
		output->clear();
		execute(program, 9, output);
		REQUIRE(output->size() > 0);
		CHECK(output->front() == 1001);
	}
}