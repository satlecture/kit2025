extern "C" {
    #include "ipasir.h"
}

#include "util/CNFFormula.h"

int getDualRailName(Lit lit) {
	return lit.sign() ? 2*lit.var()-1 : 2*lit.var();
}

int main(int argc, char **argv) {
	std::cout << "Using the incremental SAT solver " << ipasir_signature() << std::endl;

	if (argc != 2) {
		std::cout << "USAGE: ./essential <path/to/dimacs/cnf/formula>" << std::endl;
		return 0;
	}

	char* filename = argv[1];
	
	std::cout << "Reading formula from file " << filename << std::endl;
	CNFFormula formula(filename);

	std::cout << "Transforming formula using the dual rail encoding." << std::endl;
	void *solver = ipasir_init();
	for (Cl* clause : formula) {
		for (Lit& lit : *clause) {
			ipasir_add(solver, getDualRailName(lit));
		}
		ipasir_add(solver, 0);
	}

	for (int var = 1; var <= formula.nVars(); var++) {
		ipasir_add(solver, -getDualRailName(Lit(var, false)));
		ipasir_add(solver, -getDualRailName(Lit(var, true)));
		ipasir_add(solver, 0);
	}

	std::cout << "Solving the transformed formula." << std::endl;
	int result = ipasir_solve(solver);

	if (result == 20) {
		std::cout << "The formula is unsatisfiable." << std::endl;
		return 0;
	}

	std::vector<int> essentialVars;
	for (int var = 1; var <= formula.nVars(); var++) {
		std::cout << "Testing if variable " << var << " / " << formula.nVars() << " is essential: ";

		ipasir_assume(solver, -getDualRailName(Lit(var, false)));
		ipasir_assume(solver, -getDualRailName(Lit(var, true)));
		result = ipasir_solve(solver);

		if (result == 10) {
			std::cout << "NO" << std::endl;
		} else {
			std::cout << "YES" << std::endl;
			essentialVars.push_back(var);
		}
	}

	std::cout << essentialVars.size() << " out of " << formula.nVars() << " variables are essential." << std::endl;

	ipasir_release(solver);

	return 0;
}