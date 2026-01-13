/**
 * @brief Simple class to allocate variables in a structured way.
 * @author Ashlin Iser
 * @license MIT
 */

#pragma once

#include <memory>
#include <vector>

class VariableAllocator {
    unsigned next = 1;

    // remember allocations to clean them up later
    std::shared_ptr<std::vector<unsigned*>> alloc1 {new std::vector<unsigned*>()};
    std::shared_ptr<std::vector<unsigned**>> alloc2 {new std::vector<unsigned**>()};
    std::shared_ptr<std::vector<unsigned***>> alloc3 {new std::vector<unsigned***>()};

public:
    unsigned allocate() {
        return next++;
    }

    unsigned* allocate(unsigned n) {
        unsigned* result = new unsigned[n];
        alloc1->push_back(result);
        for (unsigned i = 0; i < n; i++) {
            result[i] = allocate();
        }
        return result;
    }

    unsigned** allocate(unsigned n, unsigned m) {
        unsigned** result = new unsigned*[n];
        alloc2->push_back(result);
        for (unsigned i = 0; i < n; i++) {
            result[i] = allocate(m);
        }
        return result;
    }

    unsigned*** allocate(unsigned n, unsigned m, unsigned o) {
        unsigned*** result = new unsigned**[n];
        alloc3->push_back(result);
        for (unsigned i = 0; i < n; i++) {
            result[i] = allocate(m, o);
        }
        return result;
    }

    ~VariableAllocator() {
        for (auto mem : *alloc1) delete[] mem;
        for (auto mem : *alloc2) delete[] mem;
        for (auto mem : *alloc3) delete[] mem;
        alloc1->clear();
        alloc2->clear();
        alloc3->clear();
    }
};