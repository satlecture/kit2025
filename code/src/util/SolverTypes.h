/*************************************************************************************************
SolverTypes -- Copyright (c) 2024, Markus Iser, KIT - Karlsruhe Institute of Technology

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT
OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*************************************************************************************************/

#ifndef SRC_UTIL_SOLVERTYPES_H_
#define SRC_UTIL_SOLVERTYPES_H_

#include <stdint.h>
#include <vector>
#include <iostream>

struct Var {
    unsigned id;

    Var(): id(0) { }
    explicit Var(uint32_t id_): id(id_) { }
    Var(const Var& v): Var(v.id) { }

    Var& operator= (const Var& v) {
        this->id = v.id;
        return *this;
    }

    inline operator int() const {
        return id;
    }

    inline Var& operator++ () {  // prefix ++
        ++id;
        return *this;
    }

    inline Var operator++ (int) {  // postfix ++
        Var result(*this);
        ++id;
        return result;
    }
};

struct Lit {
    unsigned x;

    Lit() : x(0) { }
    Lit(unsigned var_id, bool sign) : x(2 * var_id + sign) { }
    Lit(Var var, bool sign) : Lit(var.id, sign) { }
    explicit Lit(unsigned var_id) : Lit(var_id, false) { }
    explicit Lit(Var var) : Lit(var, false) { }

    inline operator int() const {
        return x;
    }

    inline bool sign() const {
        return x & 1;
    }

    inline Var var() const {
        return Var(x >> 1);
    }

    inline Lit positive() const {
        return Lit(var(), false);
    }

    inline Lit negative() const {
        return Lit(var(), true);
    }

    inline int toDimacs() {
        if (sign()) {
            return (-1)*var();
        } else {
            return var();
        }
    }

    inline Lit operator~ () const {
        Lit q;
        q.x = x ^ 1;
        return q;
    }

    inline Lit operator^ (bool b) const {
        Lit q;
        q.x = x ^ (b ? 1 : 0);
        return q;
    }

    inline bool operator== (Lit p) const {
        return x == p.x;
    }

    inline bool operator!= (Lit p) const {
        return x != p.x;
    }

    inline bool operator< (Lit p) const {
        return x < p.x;
    }  // p and ~p will be adjacent

    inline Lit& operator++ () {
        ++x;
        return *this;
    }

    inline Lit& operator-- () {
        --x;
        return *this;
    }
};

typedef std::vector<Lit> Cl;
typedef std::vector<Cl*> For;

inline std::ostream& operator <<(std::ostream& stream, Var const& var) {
    stream << var.id;
    return stream;
}

inline std::ostream& operator <<(std::ostream& stream, Lit const& lit) {
    if (lit.sign()) stream << "-";
    stream << lit.var();
    return stream;
}

inline std::ostream& operator <<(std::ostream& stream, Cl const& clause) {
    for (Lit lit : clause) {
        stream << lit << " ";
    }
    return stream;
}

inline std::ostream& operator <<(std::ostream& stream, For const& formula) {
    for (const Cl* clause : formula) {
        stream << *clause << std::endl;
    }
    return stream;
}

#endif  // SRC_UTIL_SOLVERTYPES_H_
