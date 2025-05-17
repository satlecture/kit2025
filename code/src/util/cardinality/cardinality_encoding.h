
#pragma once

#include <functional>

#include "../VariableAllocator.h"

void cardinality_encoding_add_literal(int lit, void* instance);
void cardinality_encoding_add_assumption(int lit, void* instance);

struct ObjectiveTerm {
    int lit;
    unsigned long factor;
};

class CardinalityEncoding {
public:
    CardinalityEncoding(VariableAllocator& va, const std::vector<int>& unweightedObjectiveLits) : _va(va) {}
    CardinalityEncoding(VariableAllocator& va, const std::vector<ObjectiveTerm>& objective) : _va(va) {}
    void setClauseCollector(std::function<void(int)> clauseCollector) {
        _clause_collector = clauseCollector;
    }
    void setAssumptionCollector(std::function<void(int)> assumptionCollector) {
        _assumption_collector = assumptionCollector;
    }
    void encodeConstraints(size_t lb, size_t ub) {
        doEncode(lb, ub);
    }
    void enforceUpperBound(size_t bound) {
        doEnforce(bound);
    }
    virtual ~CardinalityEncoding() {}
protected:
    VariableAllocator& _va;
    std::function<void(int)> _clause_collector;
    std::function<void(int)> _assumption_collector;
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
