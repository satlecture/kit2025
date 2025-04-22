#include <iostream>
#include <string>
#include <vector>

#include "util/CNFFormula.h"
#include "util/VariableAllocator.h"

#include "ipasir.h"

CNFFormula encode_seqential_counter(VariableAllocator& va, std::vector<Lit> lits, unsigned k) {
    CNFFormula f;

    unsigned n = lits.size();

    unsigned** s = va.allocate(n-1, k);

    f.readClause({ ~(lits[0]), Lit(s[0][0]) });
    for (unsigned i = 0; i < k; i++) {
        f.readClause({ ~Lit(s[0][i]) });
    }

    for (unsigned i = 1; i < n-1; i++) {
        f.readClause({ ~lits[i], Lit(s[i][0]) });
        f.readClause({ ~Lit(s[i-1][0]), Lit(s[i][0]) });
        for (unsigned j = 1; j < k; j++) {
            f.readClause({ ~lits[i], ~Lit(s[i-1][j-1]), Lit(s[i][j]) });
            f.readClause({ ~Lit(s[i-1][j]), Lit(s[i][j]) });
        }
        f.readClause({ ~lits[i], ~Lit(s[i-1][k-1]) });
    }
    f.readClause({ ~lits[n-1], Lit(s[n-2][k-1]) });

    return f;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <npigeons>" << std::endl;
        return 1;
    }

    int npigeons = std::stoi(argv[1]);
    int nholes = npigeons - 1;

    std::cout << "Testing if " << npigeons << " pigeons can be placed in " << nholes << " holes." << std::endl;

    void* solver = ipasir_init();

    VariableAllocator va;
    unsigned** p2h = va.allocate(npigeons, nholes);

    // Each pigeon is placed in at least one hole
    for (int i = 0; i < npigeons; i++) {
        for (int j = 0; j < nholes; j++) {
            ipasir_add(solver, p2h[i][j]);
        }
        ipasir_add(solver, 0);
    }

    // Each hole can only contain one pigeon
    for (int j = 0; j < nholes; j++) {
        for (int i = 0; i < npigeons; i++) {
            for (int k = i + 1; k < npigeons; k++) {
                ipasir_add(solver, -p2h[i][j]);
                ipasir_add(solver, -p2h[k][j]);
                ipasir_add(solver, 0);
            }
        }
    }

    int res = ipasir_solve(solver);

    if (res == 10) {
        std::cout << "Satisfiable." << std::endl;
    } else if (res == 20) {
        std::cout << "Unsatisfiable." << std::endl;
    } else {
        std::cout << "Unknown." << std::endl;
    }

    return 0;
}