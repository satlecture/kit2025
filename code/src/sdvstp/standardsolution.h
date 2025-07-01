
#pragma once

#include <cassert>
#include <iostream>
#include <map>
#include <vector>

// Helper classes and methods for SDVSTP. Look inside sdvstp.h for the entire interface.
#include "sdvstp.h"

#include "../util/VariableAllocator.h"
#include "../util/pb/cardinality_encoding.h"
#include "../util/pb/warners_adder.h"

extern "C" {
    #include "../ipasir.h"
}

// SAT-based SDVSTP solver to retrieve a solution.
std::vector<Action> solveSDVSTPStandardSolution(const Grid& grid) {

    void* solver = ipasir_init();
    VariableAllocator va;

    std::vector<Pattern> allPatterns = {
        P_BASE, P_COPPER, P_COPPER.getRotated(),
        P_IRON, P_IRON.getRotated(), P_GOLD,
        P_IRIDIUM, P_IRIDIUM.getRotated()
    };
    std::map<Pattern, int> patternToIdx;
    for (int p = 0; p < allPatterns.size(); p++) patternToIdx[allPatterns[p]] = p;

    auto varsTilled = va.allocate(grid.width(), grid.height());
    auto varsPattern = va.allocate(allPatterns.size(), grid.width(), grid.height());

    // All "to be tilled" cells must be tilled; all "not to be tilled"
    // cells must not be tilled.
    for (int y = 0; y < grid.height(); y++) {
        for (int x = 0; x < grid.width(); x++) {
            if (grid.at(x, y) == TO_BE_TILLED) {
                ipasir_add(solver, varsTilled[x][y]);
                ipasir_add(solver, 0);
            }
            if (grid.at(x, y) == NOT_TO_BE_TILLED) {
                ipasir_add(solver, -varsTilled[x][y]);
                ipasir_add(solver, 0);
            }
        }    
    }

    std::map<std::pair<int, int>, std::vector<Action>> effectingActions;
    std::vector<int> actionVars;

    // A specific action at a specific position implies the tilled spots.
    for (int y = 0; y < grid.height(); y++) {
        for (int x = 0; x < grid.width(); x++) {
            for (int p = 0; p < allPatterns.size(); p++) {
                auto pattern = allPatterns[p];
                Action a {pattern, false, x, y};
                if (!grid.applicable(a)) {
                    ipasir_add(solver, -varsPattern[p][x][y]);
                    ipasir_add(solver, 0);
                    continue;
                }
                actionVars.push_back(varsPattern[p][x][y]);
                for (int offX = 0; offX < pattern.width; offX++) {
                    for (int offY = 0; offY < pattern.height; offY++) {
                        ipasir_add(solver, -varsPattern[p][x][y]);
                        ipasir_add(solver, varsTilled[x+offX][y+offY]);
                        ipasir_add(solver, 0);
                        effectingActions[{x+offX, y+offY}].push_back(a);
                    }
                }
            }
        }
    }

    // A newly tilled spot implies some corresponding action supporting this change.
    for (auto [pos, actions] : effectingActions) {
        auto [x, y] = pos;
        if (grid.at(x, y) == TILLED) continue; // already tilled
        ipasir_add(solver, -varsTilled[x][y]);
        for (auto a : actions) {
            ipasir_add(solver, varsPattern[patternToIdx[a.p]][a.x][a.y]);
        }
        ipasir_add(solver, 0);
    }

    std::vector<int> assumptions;

    CardinalityEncoding* enc = new WarnersAdder(solver, va, actionVars);
    enc->encodeConstraints(0, actionVars.size());

    int lb = -1;
    int res = 20;
    while (res == 20) {
        lb++;
        std::cout << "c testing k=" << lb << std::endl;
        assumptions.clear();
        enc->enforceUpperBound(lb);
        for (int lit : assumptions) ipasir_assume(solver, lit);
        res = ipasir_solve(solver);
    }
    assert(res == 10);

    std::vector<Action> solution;
    for (int y = 0; y < grid.height(); y++) {
        for (int x = 0; x < grid.width(); x++) {
            for (int p = 0; p < allPatterns.size(); p++) {
                if (ipasir_val(solver, varsPattern[p][x][y]) <= 0) continue;
                Action a {allPatterns[p], false, x, y};
                for (auto origPattern : PATTERNS) {
                    if (a.p == origPattern)
                        solution.push_back({origPattern, false, x, y});
                    else if (a.p.getRotated() == origPattern)
                        solution.push_back({origPattern, true, x, y});
                }
            }
        }
    }
    return solution;
}