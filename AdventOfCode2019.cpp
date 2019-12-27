// AdventOfCode2019.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <iosfwd>
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"
#include <iostream>
#include <cstdint>
#include "inputReader.h"
#include "Day01/Day01.h"
#include "Day02/Day02.h"

int main(int argc, char** argv) {
    doctest::Context context;

    // !!! THIS IS JUST AN EXAMPLE SHOWING HOW DEFAULTS/OVERRIDES ARE SET !!!

    // defaults
    context.addFilter("test-case-exclude", "*math*"); // exclude test cases with "math" in their name
    context.setOption("abort-after", 5);              // stop test execution after 5 failed assertions
    context.setOption("order-by", "name");            // sort the test cases by their name

    context.applyCommandLine(argc, argv);

    // overrides
    context.setOption("no-breaks", true);             // don't break in the debugger when assertions fail

    int res = context.run(); // run

    if (context.shouldExit()) // important - query flags (and --exit) rely on the user doing this
        return res;          // propagate the result of the tests

    int client_stuff_return_code = 0;

    // your program - if the testing framework is integrated in your production code
    std::cout << "Day 01 A=" << Day01::Solve_A() << " B=" << Day01::Solve_B() << "\n";
    std::cout << "Day 02 A=" << Day02::Solve_A() << " B=" << Day02::Solve_B() << "\n";

    return res + client_stuff_return_code; // the result from doctest is propagated here as well
}