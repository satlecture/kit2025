
#include <cassert>
#include <iostream>
#include <memory>
#include <vector>

// Helper classes and methods for SDVSTP and SDVSTPP. Look inside sdvstp.h for the entire interface.
#include "sdvstp.h"

#include "../util/VariableAllocator.h"
#include "../util/pb/cardinality_encoding.h"
#include "../util/pb/warners_adder.h"
extern "C" {
    #include "../ipasir.h"
}

// TODO Implement a SAT-based SDVSTPP planner that finds an optimal plan.
std::vector<SDVSTPPAction> solveSDVSTPP(const Grid& grid) {

    // Here's some initial example code you can but don't need to use.

    //void* solver = ipasir_init();
    //VariableAllocator va;
    // ...

    // If you need to encode an at-most-k constraint, you may use the following utility
    // which implements a rather basic adder tree.
    // (Feel free to write different/better encoders and add them as a new sub-class of CardinalityEncoding.)
    //std::vector<int> literalsToConstrain; // = ...;
    //std::unique_ptr<CardinalityEncoding> enc {new WarnersAdder(solver, va, literalsToConstrain)};
    //enc->encodeConstraints(0, literalsToConstrain.size()); // adds clauses;  args = value range to encode
    // ...
    //int kToTest; // = ...;
    //enc->enforceUpperBound(kToTest); // sets assumptions
    //int res = ipasir_solve(solver);
    // ...
    // kToTest = ...;
    //enc->enforceUpperBound(kToTest); // sets assumptions
    //res = ipasir_solve(solver);
    // ...

    // Dummy return value. (Correct solution for /exercises/sdvstp/example2.sdvstp)
    return {
        {SDVSTPPAction::MOVE, SDVSTPPAction::RIGHT},
        {SDVSTPPAction::TILL_4, SDVSTPPAction::DOWN},
        {SDVSTPPAction::TILL_2, SDVSTPPAction::RIGHT},
        {SDVSTPPAction::MOVE, SDVSTPPAction::RIGHT},
        {SDVSTPPAction::MOVE, SDVSTPPAction::RIGHT},
        {SDVSTPPAction::TILL_2, SDVSTPPAction::LEFT},
        {SDVSTPPAction::TILL_1, SDVSTPPAction::DOWN}
    };
}

int main(int argc, char* argv[]) {

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    const std::string filename = argv[1];
    const auto grid = Grid::parse(filename);

    // Display grid
    grid.print();

    // Find optimal solution
    auto solution = solveSDVSTPP(grid);

    // Obtain the grid resulting from applying all actions.
    auto vGrid = grid;
    std::pair<int, int> playerPos {0, 0};
    for (auto a : solution) playerPos = vGrid.apply(playerPos.first, playerPos.second, a);
    vGrid.print();

    // Validate the soundness (not optimality!) of the solution.
    for (int y = 0; y < grid.height(); y++) {
        for (int x = 0; x < grid.width(); x++) {
            if (grid.at(x, y) == NOT_TO_BE_TILLED && vGrid.at(x, y) == TILLED) {
                throw std::runtime_error("Cell (" + std::to_string(x) + "," + std::to_string(y)
                    + ") was tilled but should not be!");
            }
            if (grid.at(x, y) == TO_BE_TILLED && vGrid.at(x, y) != TILLED) {
                throw std::runtime_error("Cell (" + std::to_string(x) + "," + std::to_string(y)
                    + ") was not tilled but should be!");
            }
        }   
    }
    std::cout << "c Solution passed soundness check." << std::endl;

    // Output the solution.
    std::cout << "s";
    for (auto a : solution) {
        std::cout << " ";
        if (a.type == SDVSTPPAction::MOVE) {
            if (a.dir == SDVSTPPAction::UP) std::cout << "U";
            if (a.dir == SDVSTPPAction::DOWN) std::cout << "D";
            if (a.dir == SDVSTPPAction::LEFT) std::cout << "L";
            if (a.dir == SDVSTPPAction::RIGHT) std::cout << "R";
        } else {
            if (a.dir == SDVSTPPAction::UP) std::cout << "u";
            if (a.dir == SDVSTPPAction::DOWN) std::cout << "d";
            if (a.dir == SDVSTPPAction::LEFT) std::cout << "l";
            if (a.dir == SDVSTPPAction::RIGHT) std::cout << "r";
            if (a.type == SDVSTPPAction::TILL_1) std::cout << "1";
            if (a.type == SDVSTPPAction::TILL_2) std::cout << "2";
            if (a.type == SDVSTPPAction::TILL_3) std::cout << "3";
            if (a.type == SDVSTPPAction::TILL_4) std::cout << "4";
            if (a.type == SDVSTPPAction::TILL_5) std::cout << "5";
        }
    }
    std::cout << std::endl;
}
