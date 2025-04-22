// Encode pigeonhole principle as a SAT problem

#include <iostream>
#include <vector>
#include <string>

#include "util/CNFFormula.h"

class VariableAllocator {
    unsigned next_var;

public:
    VariableAllocator() : next_var(1) {}

    unsigned allocate() {
        return next_var++;
    }

    unsigned* allocate1D(int n) {
        unsigned* result = new unsigned[n];
        for (unsigned i = 0; i < n; i++) {
            result[i] = allocate();
        }
        return result;
    }

    unsigned** allocate2D(unsigned n, unsigned m) {
        unsigned** result = new unsigned*[n];
        for (unsigned i = 0; i < n; i++) {
            result[i] = new unsigned[m];
            for (unsigned j = 0; j < m; j++) {
                result[i][j] = allocate();
            }
        }
        return result;
    }
};

CNFFormula encodePigeonholeDirect(unsigned n, unsigned k) {
    VariableAllocator va {};
    CNFFormula f;

    unsigned** vars = va.allocate2D(n, k);

    // Each pigeon is in at least one hole
    std::vector<Lit> clause;
    for (unsigned i = 0; i < n; i++) {
        for (unsigned j = 0; j < k; j++) {
            clause.push_back(Lit(vars[i][j]));
        }
        f.readClause(clause.begin(), clause.end());
        clause.clear();
    }

    // Each hole has at most one pigeon
    for (unsigned j = 0; j < k; j++) {
        for (unsigned i1 = 0; i1 < n; i1++) {
            for (unsigned i2 = i1 + 1; i2 < n; i2++) {
                f.readClause({ Lit(vars[i1][j], true), Lit(vars[i2][j], true) });
            }
        }
    }

    return f;
}

// encode sum(lits) <= k
CNFFormula encode_seqential_counter(VariableAllocator& va, std::vector<Lit> lits, unsigned k) {
    CNFFormula f;

    unsigned n = lits.size();

    unsigned** s = va.allocate2D(n-1, k);

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

CNFFormula encodePigeonholeSequentialCounter(int n, int k) {
    VariableAllocator va {};
    CNFFormula f;

    unsigned** vars = va.allocate2D(n, k);

    // Each pigeon is in at least one hole
    std::vector<Lit> clause;
    for (unsigned i = 0; i < n; i++) {
        for (unsigned j = 0; j < k; j++) {
            clause.push_back(Lit(vars[i][j]));
        }
        f.readClause(clause.begin(), clause.end());
        clause.clear();
    }

    // Each hole has at most one pigeon
    for (unsigned j = 0; j < k; j++) {
        std::vector<Lit> lits {};
        for (unsigned i = 0; i < n; i++) {
            lits.push_back(Lit(vars[i][j]));
        }
        CNFFormula f2 = encode_seqential_counter(va, lits, 1);
        f.append(f2);
    }

    return f;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <p>" << std::endl;
        return 1;
    }
    int p = std::stoi(argv[1]);
    CNFFormula f = encodePigeonholeSequentialCounter(p, p-1);
    f.printDimacs(std::cout);
    return 0;
}