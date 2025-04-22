/**
 * @brief Trail class
 * @author Markus Iser
 * @license MIT
 */

#include <vector>
#include "util/SolverTypes.h"
#include "util/CNFFormula.h"

class Trail {

    const bool verb = false;

    std::vector<Lit> trail; // Trail
    std::vector<unsigned> trail_lim; // Stack of trail limits
    std::vector<uint8_t> val; // Assignment (0 = unassigned, 1 = true, 2 = false)
    std::vector<For> occ; // Occurence list
    int head = 0; // Head of the trail which is not yet propagated

public:
    // value constants
    const uint8_t UNASSIGNED = 0;
    const uint8_t TRUE = 1;
    const uint8_t FALSE = 2;

    Trail() : occ(), val(), trail(), trail_lim() { }

    void clear() {
        occ.clear();
        val.clear();
        trail.clear();
        trail_lim.clear();
    }

    bool init(CNFFormula& formula) {
        clear();
        occ.resize(2*formula.nVars()+2);
        val.resize(formula.nVars()+1);
        trail_lim.push_back(0);
        for (Cl* cls : formula) {
            if (cls->size() == 1) {
                if (verb) std::cout << "Unit clause: " << *cls << std::endl;
                if (!push((*cls)[0])) return false;
            }
            for (Lit lit : *cls) {
                occ[lit].push_back(cls);
            }
        }
        return propagate();
    }

    bool propagate() {
        while (head < trail.size()) {
            Lit lit = trail[head];
            head++;
            for (Cl* cls : occ[~lit]) {
                if (verb) std::cout << "Checking clause: " << *cls << std::endl;
                Lit unit = Lit();
                int count = 0;
                for (Lit lit : *cls) {
                    if (val[lit.var()] == lit.sign() + 1) {
                        if (verb) std::cout << "Clause is satisfied." << std::endl;
                        unit = Lit();
                        break; // clause is satisfied
                    } 
                    else if (val[lit.var()] == UNASSIGNED) {
                        if (unit != 0) {
                            if (verb) std::cout << "More than one unassigned literal." << std::endl;
                            unit = Lit();
                            break; // more than one unassigned literal
                        }
                        unit = lit;
                    }
                    else {
                        if (verb) std::cout << "Literal " << lit << " is false." << std::endl;
                        count++;
                    }
                }
                if (unit != 0 && !push(unit) || count == cls->size()) {
                    return false;
                }
            }
        }
        return true;
    }

    uint8_t value(Var var) {
        return val[var];
    }

    bool push(Lit lit) {
        if (verb) std::cout << "Pushing " << lit << std::endl;
        uint8_t new_val = lit.sign() ? FALSE : TRUE;
        if (val[lit.var()] != 0) {
            return val[lit.var()] == new_val;
        }
        val[lit.var()] = new_val;
        trail.push_back(lit);
        if (verb) {
            std::cout << "Trail: ";
            for (Lit l : trail) {
                std::cout << l << " ";
            }
            std::cout << std::endl;
        }
        return true;
    }

    void newLevel() {
        trail_lim.push_back(trail.size());
    }

    bool backtrack() {
        if (!trail_lim.empty()) {
            if (verb) std::cout << "Backtracking to position " << trail_lim.back() << std::endl;
            for (int i = trail.size()-1; i >= (int)trail_lim.back(); i--) {
                if (verb) std::cout << "Unassigning " << trail[i] << " at position " << i << std::endl;
                val[trail[i].var()] = UNASSIGNED;
            }
            trail.resize(trail_lim.back());
            trail_lim.pop_back();
            head = trail.size();
            return true;
        }
        return false;
    }

    Lit operator[](int i) {
        return trail[i];
    }

    int size() {
        return trail.size();
    }

    // Print the trail
    void print() {
        for (Lit lit : trail) {
            std::cout << lit << " ";
        }
        std::cout << std::endl;
    }

    // Test if trail satisfies the formula
    bool satisfies(CNFFormula& formula) {
        for (Cl* cls : formula) {
            bool sat = false;
            for (Lit lit : *cls) {
                if (val[lit.var()] == lit.sign() + 1) {
                    sat = true;
                    break;
                }
            }
            if (!sat) {
                return false;
            }
        }
        return true;
    }
};