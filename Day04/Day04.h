#pragma once
#include "../inputReader.h"

namespace Day04
{
	auto start{ 240298 };
	auto end{ 784956 };

	//	t is a six-digit number.
	// The value is within the range given in your puzzle input.
	// Two adjacent digits are the same (like 22 in 122345).
	// Going from left to right, the digits never decrease; they only ever increase or stay the same (like 111123 or 135679).
	bool checkIfOK(uint32_t n)
	{
		auto s = std::to_string(n);

		//	t is a six-digit number.
		if (s.length() != 6) return false;

		// Two adjacent digits are the same (like 22 in 122345).
		if (s[0] == s[1] || s[1] == s[2] || s[2] == s[3] || s[3] == s[4] || s[4] == s[5])
		{
			// Going from left to right, the digits never decrease
			if (s[0] <= s[1] && s[1] <= s[2] && s[2] <= s[3] && s[3] <= s[4] && s[4] <= s[5])
			{
				return true;
			}
		}
		return false;
	}

	TEST_CASE("checkIfOK") {
		CHECK(checkIfOK(111111) == true);
		CHECK(checkIfOK(223450) == false);
		CHECK(checkIfOK(123789) == false);
	}

	// 111111 meets these criteria (double 11, never decreases).
	// 223450 does not meet these criteria (decreasing pair of digits 50).
	// 123789 does not meet these criteria (no double).

	int64_t Solve_A()
	{
		//auto	file = aoc::OpenInputFile("Day03\\Day03.txt");
		//const auto	input = aoc::ReadInput(*file);
		auto count{ 0 };
		for (auto i = start; i <= end; ++i)
		{
			if (checkIfOK(i)) ++count;
		}
		return 	count;
	}

	bool checkIfOK_B(uint32_t n)
	{
		if (!checkIfOK(n)) return false;

		auto s = std::to_string(n);

		// the two adjacent matching digits are not part of a larger group of matching digits.
		if (   s[0] == s[1] && s[1] != s[2]
			|| s[1] == s[2] && s[0] != s[1] && s[2] != s[3]
			|| s[2] == s[3] && s[1] != s[2] && s[3] != s[4]
			|| s[3] == s[4] && s[2] != s[3] && s[4] != s[5]
			|| s[4] == s[5] && s[3] != s[4]) return true;

		return false;
	}

	// the two adjacent matching digits are not part of a larger group of matching digits.
	TEST_CASE("checkIfOK_B") {
		CHECK(checkIfOK_B(112233) == true);
		CHECK(checkIfOK_B(123444) == false);
		CHECK(checkIfOK_B(111122) == true);
	}


	int64_t Solve_B()
	{
		//auto	file = aoc::OpenInputFile("Day03\\Day03.txt");
		//const auto	input = aoc::ReadInput(*file);
		auto count{ 0 };
		for (auto i = start; i <= end; ++i)
		{
			if (checkIfOK_B(i)) ++count;
		}
		return 	count;
	}


}