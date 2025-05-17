
#include <cassert>
#include <iostream>
#include <map>
#include <memory>
#include <vector>

// Helper classes and methods for SDVSTP. Look inside sdvstp.h for the entire interface.
#include "sdvstp.h"

// Potentially useful includes
#include "../util/VariableAllocator.h"
#include "../util/cardinality/cardinality_encoding.h"
#include "../util/cardinality/warners_adder.h"
extern "C" {
    #include "../ipasir.h"
}

// TODO Implement a SAT-based SDVSTP solver that finds an optimal solution.
std::vector<Action> solveSDVSTP(const Grid& grid) {

    // void* solver = ipasir_init();
    // VariableAllocator va;
    // ...

    // If you need to encode an at-most-k constraint, you can use the provided utility:
    // std::vector<int> literalsToConstrain = ...;
    // std::unique_ptr<CardinalityEncoding> enc;
    // enc.reset(new WarnersAdder(va, literalsToConstrain));
    // enc->setClauseCollector([&](int lit) {ipasir_add(solver, lit);});
    // enc->setAssumptionCollector([&](int lit) {ipasir_assume(solver, lit);});
    // enc->encodeConstraints(0, literalsToConstrain.size()); // adds clauses;  args = value range to encode
    // ...
    // int kToTest;
    // enc->enforceUpperBound(kToTest); // sets assumptions
    // int res = ipasir_solve(solver);
    // ...
    // kToTest = ...;
    // enc->enforceUpperBound(kToTest); // sets assumptions
    // res = ipasir_solve(solver);
    // ...

    // Dummy return value.
    // Each item in the returned vector must feature one of the five patterns P_*.
    return {
        {P_GOLD,   false, 0, 0},
        {P_GOLD,   false, 5, 1},
        {P_IRON,   true,  3, 0},
        {P_COPPER, true,  3, 1},
        {P_BASE,   false, 0, 3},
        {P_BASE,   0,     7, 4}
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
    auto solution = solveSDVSTP(grid);

    // Obtain the grid resulting from applying all actions.
    auto vGrid = grid;
    for (auto a : solution) vGrid.apply(a);
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
    std::cout << "s SOLUTION " << solution.size() << std::endl;
    for (auto a : solution) {
        if (a.p == P_BASE)    std::cout << "v base ";
        if (a.p == P_COPPER)  std::cout << "v copper ";
        if (a.p == P_IRON)    std::cout << "v iron ";
        if (a.p == P_GOLD)    std::cout << "v gold ";
        if (a.p == P_IRIDIUM) std::cout << "v iridium ";
        std::cout << (a.rotate ? "90" : "0") << " " << a.x << " " << a.y << std::endl;
    }
}
