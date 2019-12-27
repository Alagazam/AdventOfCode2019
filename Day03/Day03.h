#pragma once
#include <sstream>
#include "../inputReader.h"
namespace Day03
{
	auto  ex1{ "R8,U5,L5,D3\n"
			   "U7,R6,D4,L4" };
	auto  dist1{ 6 };

	auto  ex2{ "R75,D30,R83,U83,L12,D49,R71,U7,L72\n"
				"U62,R66,U55,R34,D71,R55,D58,R83" };
	auto dist2{ 159 };
	auto  ex3{ "R98,U47,R26,D63,R33,U87,L62,D20,R33,U53,R51\n"
				"U98,R91,D20,R16,D67,R40,U7,R15,U6,R7" };
	auto dist3{ 135 };

	struct point {
		int x{ 0 };
		int y{ 0 };
		bool operator<(const point& rhs) const { return (x < rhs.x) ? true : ((x == rhs.x) ? (y < rhs.y) : false); };
	};
	TEST_CASE("point <") {
		point p1{ 0,0 };
		point p2{ 0,2 };
		point p3{ 2,2 };

		CHECK( (p1<p1) == false);
		CHECK( p1 < p2 );
		CHECK( p1 < p3 );
		CHECK( p2 < p3 );
	}

	using path = std::set<point>;

	path	makePath(aoc::rowContent wire)
	{
		path	res;
		point	pos;
		for (auto move : wire)
		{
			auto direction{ move[0] };
			auto dist{ std::stoi(move.substr(1)) };
			for (auto step = 0; step != dist; ++step)
			{
				switch (direction) {
				case 'R':	++pos.x; break;
				case 'U':	++pos.y; break;
				case 'D':	--pos.y; break;
				case 'L':	--pos.x; break;
				}
				res.insert(pos);
			}
		}
		return res;
	}
	TEST_CASE("makePath ex1") {
		std::istringstream  iss{ ex1 };
		auto wire{ aoc::ReadInput(iss) };
		auto res = makePath(wire[0]);
		CHECK(res.size() == 21);
		CHECK(res.find(point{ 3,2 }) != res.end());
	}

	int64_t Solve_A()
	{
		auto	file = aoc::OpenInputFile("Day03\\Day03.txt");
		auto	input = aoc::ReadInput(*file);

		return 0;
	}

	int64_t Solve_B()
	{
		auto	file = aoc::OpenInputFile("Day03\\Day03.txt");
		auto	input = aoc::ReadInput(*file);

		return 0;
	}


}