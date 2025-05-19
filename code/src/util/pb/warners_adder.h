
#pragma once

#include <functional>
#include <cassert>

#include "cardinality_encoding.h"
#include "enc_adder.h"

class WarnersAdder : public CardinalityEncoding {

public:

    WarnersAdder(void* ipasirSolver, VariableAllocator& va, const std::vector<int>& unweightedObjectiveLits) :
            CardinalityEncoding(ipasirSolver, va, unweightedObjectiveLits), _wbo_solver(this, va) {

        for (int lit : unweightedObjectiveLits) {
            _lits.push_back(lit);
            _weights.push_back(1);
            _sum_of_weights++;
        }
    }
    WarnersAdder(void* ipasirSolver, VariableAllocator& va, const std::vector<ObjectiveTerm>& objective) :
            CardinalityEncoding(ipasirSolver, va, objective), _wbo_solver(this, va) {

        for (auto& term : objective) {
            _lits.push_back(term.lit);
            _weights.push_back(term.factor);
            _sum_of_weights += term.factor;
        }
    }

    virtual void doEncode(size_t lb, size_t ub) override {
        if (_enc.hasCreatedEncoding()) return;
        size_t max = _sum_of_weights;
        auto internalLits = _enc.convertLiterals(_lits);
        if (_enforce_mode == EXTEND) {
            // We need to always encode the adder up until the total sum of all coefficients.
            // Otherwise, costs above the provided "max" are *not* being forbidden and can
            // be reported as valid models by a solver.
            size_t rhs = std::max(max, _sum_of_weights);
            _enc.encode(&_wbo_solver, internalLits, _weights, rhs);
        } else {
            _enc.encodeWithBitwiseAssumableBounds(&_wbo_solver, internalLits, _weights);
        }
    }

    virtual void doEnforce(size_t bound) override {
        assert(_enc.hasCreatedEncoding());
        openwbo::Adder::vec<openwbo::Lit> assumptions;
        if (_enforce_mode == EXTEND) {
            _enc.updateInc(&_wbo_solver, bound, assumptions);
        } else {
            assumptions = _enc.enforceBoundBitwise(&_wbo_solver, bound);
        }
        for (auto a : assumptions)
            cardinality_encoding_add_assumption(openwbo::toExternalLit(a), this);
    }

    virtual ~WarnersAdder() {}

private:
    std::vector<int> _lits;
    std::vector<size_t> _weights;
    size_t _sum_of_weights;

    // EXTEND: Each bound enforcement adds new permanent clauses with activation literals
    // and then activates them with a single assumption.
    // ASSUME: Each bound enforcement *only* sets a number of assumptions that enforce
    // the bits of the upper bound. The meaning of these bits is encoded from the beginning.
    enum EnforceMode {EXTEND, ASSUME} _enforce_mode {ASSUME};

    // Glue code for OpenWBO
    openwbo::Adder _enc;
    struct SolverProxy : public openwbo::Adder::Solver {
        WarnersAdder* parent;
        VariableAllocator& va;
        SolverProxy(WarnersAdder* parent, VariableAllocator& va) : parent(parent), va(va) {}
        virtual int freshVariable() override {
            return va.allocate();
        }
        virtual void pushLiteral(int lit) override {
            cardinality_encoding_add_literal(lit, parent);
        }
    } _wbo_solver;
};
