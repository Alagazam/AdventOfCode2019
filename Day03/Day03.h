#pragma once
#include <sstream>
#include "../inputReader.h"
#include <algorithm>

namespace Day03
{
	auto  ex1{ "R8,U5,L5,D3\n"
			   "U7,R6,D4,L4" };
	auto dist1{ 6 };
	auto shortest1{ 30 };

	auto  ex2{ "R75,D30,R83,U83,L12,D49,R71,U7,L72\n"
				"U62,R66,U55,R34,D71,R55,D58,R83" };
	auto dist2{ 159 };
	auto shortest2{ 610 };

	auto  ex3{ "R98,U47,R26,D63,R33,U87,L62,D20,R33,U53,R51\n"
				"U98,R91,D20,R16,D67,R40,U7,R15,U6,R7" };
	auto dist3{ 135 };
	auto shortest3{ 410 };

	struct point {
		int x{ 0 };
		int y{ 0 };
		int seq{ 0 };
		bool operator<(const point& rhs) const { return (x < rhs.x) ? true : ((x == rhs.x) ? (y < rhs.y) : false); };
		bool operator==(const point& rhs) const { return (x == rhs.x)  && (y == rhs.y); };
		int dist() const { return std::abs(x) + std::abs(y); };
	};
	TEST_CASE("point <") {
		point p1{ 0,0 };
		point p2{ 0,2 };
		point p3{ 2,2 };

		CHECK( (p1<p1) == false); //-V501
		CHECK( p1 < p2 );
		CHECK( p1 < p3 );
		CHECK( p2 < p3 );
	}

	using path = std::set<point>;

	path	makePath(const aoc::rowContent wire)
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
				++pos.seq;
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

	int64_t  findMinDistIntersection(const aoc::inputContent input)
	{
		const auto path1 = makePath(input[0]);
		const auto path2 = makePath(input[1]);
		path intersections;
		auto it = std::set_intersection(path1.begin(), path1.end(), path2.begin(), path2.end(), std::inserter(intersections, intersections.begin()));

		int mindist = std::numeric_limits<int>::max();
		for (const auto p : intersections)
		{
			const auto dist = p.dist();
			if (dist < mindist) mindist = dist;
		}
		return mindist;
	}
	TEST_CASE("findMinDistIntersection ex1") {
		std::istringstream  iss{ ex1 };
		const auto wires{ aoc::ReadInput(iss) };
		const auto res = findMinDistIntersection(wires);
		CHECK(res == dist1);
	}
	TEST_CASE("findMinDistIntersection ex2") {
		std::istringstream  iss{ ex2 };
		const auto wires{ aoc::ReadInput(iss) };
		const auto res = findMinDistIntersection(wires);
		CHECK(res == dist2);
	}
	TEST_CASE("findMinDistIntersection ex3") {
		std::istringstream  iss{ ex3 };
		const auto wires{ aoc::ReadInput(iss) };
		const auto res = findMinDistIntersection(wires);
		CHECK(res == dist3);
	}



	int64_t  findShortestIntersection(aoc::inputContent input)
	{
		const auto path1 = makePath(input[0]);
		const auto path2 = makePath(input[1]);
		path intersections;
		auto it = std::set_intersection(path1.begin(), path1.end(), path2.begin(), path2.end(), std::inserter(intersections, intersections.begin()));

		int mindist = std::numeric_limits<int>::max();
		for (const auto p : intersections)
		{
			const auto seq1 = path1.find(p)->seq;
			const auto seq2 = path2.find(p)->seq;
			auto dist = seq1 + seq2;
			if (dist < mindist) mindist = dist;
		}
		return mindist;
	}


	TEST_CASE("findShortestIntersection ex1") {
		std::istringstream  iss{ ex1 };
		const auto wires{ aoc::ReadInput(iss) };
		const auto res = findShortestIntersection(wires);
		CHECK(res == shortest1);
	}
	TEST_CASE("findShortestIntersection ex2") {
		std::istringstream  iss{ ex2 };
		const auto wires{ aoc::ReadInput(iss) };
		const auto res = findShortestIntersection(wires);
		CHECK(res == shortest2);
	}
	TEST_CASE("findShortestIntersection ex3") {
		std::istringstream  iss{ ex3 };
		const auto wires{ aoc::ReadInput(iss) };
		const auto res = findShortestIntersection(wires);
		CHECK(res == shortest3);
	}

	int64_t Solve_A()
	{
		auto	file = aoc::OpenInputFile("Day03\\Day03.txt");
		const auto	input = aoc::ReadInput(*file);
		return 	findMinDistIntersection(input);
	}

	int64_t Solve_B()
	{
		auto	file = aoc::OpenInputFile("Day03\\Day03.txt");
		const auto	input = aoc::ReadInput(*file);
		return 	findShortestIntersection(input);
	}


}