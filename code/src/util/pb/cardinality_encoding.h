
#pragma once

#include <functional>

#include "../VariableAllocator.h"
#include "../../ipasir.h"

// C-style callbacks, in case some PB encoding needs them.
void cardinality_encoding_add_literal(int lit, void* instance);
void cardinality_encoding_add_assumption(int lit, void* instance);

struct ObjectiveTerm {
    int lit;
    unsigned long factor;
};

// Helper class to construct incremental encodings that enforce constraints of the form Σ_i wi xi ≤ k,
// where xi are Boolean literals with according positive integer weights wi and k is a non-negative integer.
class CardinalityEncoding {

public:
    // Constructor for an *unweighted* constraint, i.e., all wi are exactly 1.
    CardinalityEncoding(void* ipasirSolver, VariableAllocator& va, const std::vector<int>& unweightedObjectiveLits) : _solver(ipasirSolver), _va(va) {
        setClauseCollector([&](int lit) {ipasir_add(_solver, lit);});
        setAssumptionCollector([&](int lit) {ipasir_assume(_solver, lit);});
    }
    // Constructor for a *weighted* constraint, i.e., explicit pairs of (xi, wi) are given.
    CardinalityEncoding(void* ipasirSolver, VariableAllocator& va, const std::vector<ObjectiveTerm>& objective) : _solver(ipasirSolver), _va(va) {
        setClauseCollector([&](int lit) {ipasir_add(_solver, lit);});
        setAssumptionCollector([&](int lit) {ipasir_assume(_solver, lit);});
    }
    // Encode all constraints necessary to subsequently enforce any upper bound for k in [lb, ub].
    void encodeConstraints(size_t lb, size_t ub) {
        doEncode(lb, ub);
    }
    // Enforce the constraint for k = bound, using assumptions (i.e., only for a single call).
    void enforceUpperBound(size_t bound) {
        doEnforce(bound);
    }
    virtual ~CardinalityEncoding() {}

protected:
    void* _solver;
    VariableAllocator& _va;
    std::function<void(int)> _clause_collector;
    std::function<void(int)> _assumption_collector;
    void setClauseCollector(std::function<void(int)> clauseCollector) {
        _clause_collector = clauseCollector;
    }
    void setAssumptionCollector(std::function<void(int)> assumptionCollector) {
        _assumption_collector = assumptionCollector;
    }
    virtual void doEncode(size_t lb, size_t ub) = 0;
    virtual void doEnforce(size_t bound) = 0;

private:
    void addLiteral(int lit) {
        _clause_collector(lit);
    }
    void addAssumption(int lit) {
        _assumption_collector(lit);
    }

friend void cardinality_encoding_add_literal(int lit, void* instance);
friend void cardinality_encoding_add_assumption(int lit, void* instance);
};
