#pragma once
#include "../inputReader.h"
namespace Day01
{
    int64_t CalcFuel(int64_t mass)
    {
        return mass / 3 - 2;
    };

    TEST_CASE("testing the CalcFuel function") {
        CHECK(CalcFuel(12) == 2);
        CHECK(CalcFuel(14) == 2);
        CHECK(CalcFuel(1969) == 654);
        CHECK(CalcFuel(100756) == 33583);
    }

    int64_t    SumFuel(aoc::inputLines input)
    {
        int64_t    sumFuel{ 0 };
        for (auto line : input)
        {
            auto mass = std::stoull(line);
            sumFuel += CalcFuel(mass);
        }
        return sumFuel;
    }

    TEST_CASE("testing the SumFuel function") {
        CHECK(SumFuel({ "12" }) == 2);
        CHECK(SumFuel({ "14", "12" }) == 4);
        CHECK(SumFuel({ "12","14","1969","100756" }) == 2 + 2 + 654 + 33583);
    }

    int64_t Solve_A()
    {
        auto	file = aoc::OpenInputFile("Day01\\Day01.txt");
        auto	input = aoc::ReadInputLines(*file);
        return SumFuel(input);
    }


    int64_t CalcAllFuel(int64_t mass)
    {
        auto fuel = CalcFuel(mass);
        if (fuel <= 0) return 0;
        return fuel + CalcAllFuel(fuel);

    };

    TEST_CASE("testing the CalcAllFuel function") {
        CHECK(CalcAllFuel(12) == 2);
        CHECK(CalcAllFuel(14) == 2);
        CHECK(CalcAllFuel(1969) == 966);
        CHECK(CalcAllFuel(100756) == 50346);
    }

    int64_t    SumAllFuel(aoc::inputLines input)
    {
        int64_t    sumFuel{ 0 };
        for (auto line : input)
        {
            auto mass = std::stoull(line);
            sumFuel += CalcAllFuel(mass);
        }
        return sumFuel;
    }

    TEST_CASE("testing the SumAllFuel function") {
        CHECK(SumAllFuel({ "12" }) == 2);
        CHECK(SumAllFuel({ "14", "12" }) == 4);
        CHECK(SumAllFuel({ "12","14","1969","100756" }) == 2 + 2 + 966 + 50346);
    }

    int64_t Solve_B()
    {
        auto	file = aoc::OpenInputFile("Day01\\Day01.txt");
        auto	input = aoc::ReadInputLines(*file);
        return SumAllFuel(input);
    }

}