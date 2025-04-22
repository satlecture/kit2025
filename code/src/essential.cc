extern "C" {
    #include "ipasir.h"
}

#include "util/CNFFormula.h"


int main(int argc, char **argv) {
	std::cout << "Using the incremental SAT solver " << ipasir_signature() << std::endl;

	if (argc != 2) {
		std::cout << "USAGE: ./essential <path/to/dimacs/cnf/formula>" << std::endl;
		return 0;
	}

	char* filename = argv[1];
	CNFFormula formula(filename);

	std::cout << "The formula has " << formula.nVars() << " variables and " << formula.nClauses() << " clauses." << std::endl;

	// TODO: Transform the formula using the dual-rail encoding, and determine the essential variables.

	return 0;
}