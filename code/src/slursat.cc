#include <cassert>
#include <random>

#include "util/CNFFormula.h"
#include "trail.h"


std::vector<Lit> get_permutation(size_t nVars, size_t seed) {
    std::vector<Lit> perm(nVars);
    for (int i = 0; i < nVars; i++) {
        perm[i] = Lit(i+1, false);
    }

    std::mt19937 g(seed);
    std::shuffle(perm.begin(), perm.end(), g);
    return perm;
}

// SLUR algorithm
bool slursat(std::vector<Lit> lits, Trail& trail) {
    for (Lit lit : lits) {
        if (trail.value(lit.var()) == trail.UNASSIGNED) {
            trail.newLevel();
            trail.push(lit);
            if (!trail.propagate()) {
                trail.backtrack();
                trail.newLevel();
                trail.push(~lit);
                if (!trail.propagate()) {
                    return false;
                }
            }
        }
    }
    return true;
}

int main(int argc, char **argv) {
	if (argc < 2) {
		std::cout << "USAGE: ./slur <path/to/dimacs/cnf/formula> [seed]" << std::endl;
		return 0;
	}

	CNFFormula formula(argv[1]);
    int seed = argc > 2 ? std::stoi(argv[2]) : 0; 

	std::cout << "c Testing if formula " << argv[1] << " can be solved by SLUR algorithm." << std::endl;
	
    Trail trail {};
    if (!trail.init(formula)) {
        std::cout << "s UNSATISFIABLE" << std::endl;
        return 0;
    }

    std::vector<Lit> perm = get_permutation(formula.nVars(), seed);

    if (slursat(perm, trail)) {
        std::cout << "s SATISFIABLE" << std::endl;
        trail.print();
        assert(trail.satisfies(formula));
    } else {
        std::cout << "s UNKNOWN" << std::endl;
        std::cout << "c GAVE UP" << std::endl;
    }

	return 0;
}