#pragma once
#include <vector>
#include <istream>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <iterator>
#include <memory>

template<typename T>
std::ostream& operator<< (std::ostream& os, const std::vector<T>& value) {
	for (auto val : value)
		os << val << ',';
	return os;
}

namespace aoc
{

	using rowContent = std::vector<std::string>;

	inline rowContent Tokenize(std::string line, char delim = ',')
	{
		std::istringstream iss(line);
		rowContent tokens;
		std::string item;
		while (std::getline(iss, item, delim))
		{
			tokens.push_back(item);
		}
		return tokens;
	}
	TEST_CASE("TokenizerTest, OneToken")
	{
		std::string s("Test");
		auto tokens = aoc::Tokenize(s);
		REQUIRE(1 == tokens.size());
		CHECK("Test" == tokens[0]);
	}
	TEST_CASE("TokenizerTest, TwoTokens")
	{
		std::string s("Test this");
		auto tokens = aoc::Tokenize(s, ' ');
		REQUIRE(2 == tokens.size());
		CHECK("this" == tokens[1]);
	}
	TEST_CASE("testing the Tokenize function") {
		auto	input = aoc::Tokenize("1,2,3,4");
		CHECK(input.size() == 4);
		aoc::rowContent	row{ "1","2","3","4" };
		CHECK(input == row);
	}

	using inputContent = std::vector<rowContent>;

	inline inputContent ReadInput(std::istream& s, char delim = ',')
	{
		std::string	line;
		inputContent	lines;
		while (std::getline(s, line))
		{
			auto tokens = Tokenize(line, delim);
			lines.push_back(tokens);
		}
		return lines;
	}
	TEST_CASE("ReadInputTest, ReadInputOneLine")
	{
		std::istringstream	iss("This is a Test");
		auto result = aoc::ReadInput(iss, ' ');
		REQUIRE(1 == result.size());
		REQUIRE(4 == result[0].size());
		CHECK("This" == result[0][0]);
		CHECK("a" == result[0][2]);
		CHECK("Test" == result[0][3]);
	}
	TEST_CASE("ReadInputTest, ReadInputTwoLine")
	{
		std::istringstream	iss("This is a Test\nAnd now line 2");
		auto result = aoc::ReadInput(iss, ' ');
		REQUIRE(2 == result.size());
		REQUIRE(4 == result[1].size());
		CHECK("Test" == result[0][3]);
		CHECK("And" == result[1][0]);
		CHECK("2" == result[1][3]);
	}
	TEST_CASE("ReadInputTest, ReadInputOneLineWithLineBreak")
	{
		std::istringstream	iss("This is a Test\n");
		auto result = aoc::ReadInput(iss, ' ');
		REQUIRE(1 == result.size());
		CHECK("Test" == result[0][3]);
	}
	TEST_CASE("testing the ReadInput function") {
		std::istringstream ss("1,2,3,4\n-1,-2,-3\n");
		auto	input = aoc::ReadInput(ss);

		CHECK(input.size() == 2);

		aoc::rowContent	row0{ "1","2","3","4" };
		CHECK(input[0] == row0);

		aoc::rowContent	row1{ "-1","-2","-3" };
		CHECK(input[1] == row1);
	}

	using rowContentInt = std::vector<int64_t>;

	inline rowContentInt TokenizeInt(std::string line, char delim=',')
	{
		std::istringstream iss(line);
		rowContentInt tokens;
		std::string item;
		while (std::getline(iss, item, delim))
		{
			tokens.push_back(std::stoll(item));
		}
		return tokens;
	}
	TEST_CASE("testing the TokenizeInt function") {
		auto	input = aoc::TokenizeInt("1,2,3,4");

		CHECK(input.size() == 4);

		aoc::rowContentInt	row{ 1,2,3,4 };
		CHECK(input == row);
	}

	using contentInt = std::vector<rowContentInt>;

	inline contentInt ReadInputInt(std::istream& s, char delim = ',')
	{
		std::string	line;
		contentInt	lines;
		while (std::getline(s, line))
		{
			auto tokens = TokenizeInt(line, delim);
			lines.push_back(tokens);
		}
		return lines;
	}
	TEST_CASE("testing the ReadInputInt function") {
		std::istringstream ss("1,2,3,4\n-1,-2,-3\n");
		auto	input = aoc::ReadInputInt(ss);

		CHECK(input.size() == 2);

		aoc::rowContentInt	row0{ 1,2,3,4 };
		CHECK(input[0] == row0);

		aoc::rowContentInt	row1{ -1,-2,-3 };
		CHECK(input[1] == row1);
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
	TEST_CASE("ReadInputLinesTest, ReadInputTwoLine")
	{
		std::istringstream	iss("This is a Test\nAnd now line 2");
		auto result = aoc::ReadInputLines(iss);
		REQUIRE(2 == result.size());
		CHECK("This is a Test" == result[0]);
		CHECK("And now line 2" == result[1]);
	}


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
	TEST_CASE("OpenInputFile, OpenFileAndRead")
	{
		auto file = aoc::OpenInputFile("test.txt");
		auto result = aoc::ReadInput(*file, ' ');
		REQUIRE(2 == result.size());
		REQUIRE(4 == result[1].size());
		CHECK("And" == result[1][0]);
		CHECK("2" == result[1][3]);
	}
}

namespace doctest {
	template<> struct StringMaker<std::vector<std::string>> {
		static String convert(const std::vector<std::string>& value) {
			std::string res;
			for (auto val : value)
				res += val + ',';
			return res.c_str();
		}
	};
	template<typename T> struct StringMaker<std::vector<T>> {
		static String convert(const std::vector<T>& value) {
			std::string res;
			for (auto val : value)
				res += std::to_string(val) + ',';
			return res.c_str();
		}
	};
}
