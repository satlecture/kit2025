
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "../src/util/CNFFormula.h"
#include "../src/trail.h"

TEST_CASE("Trail") {
    // SUBCASE("initpushpropagate") {
    //     CNFFormula f;
    //     f.readClause({ Lit(1), Lit(2) });
    //     Trail trail {};
    //     CHECK(trail.init(f));
    //     CHECK(trail.push(Lit(1, true)));
    //     CHECK(trail.propagate());
    //     CHECK(!trail.push(Lit(2, true)));
    //     CHECK(trail.push(Lit(2, false)));
    // }

    SUBCASE("propagate") {
        CNFFormula f;
        f.readClause({ Lit(1) });
        f.readClause({ Lit(1, true), Lit(2) });
        f.readClause({ Lit(2, true), Lit(3) });
        Trail trail {};
        CHECK(trail.init(f));
        CHECK(trail.propagate());
    }

    SUBCASE("backtrack") {
        CNFFormula f;
        f.readClause({ Lit(1, true), Lit(2) });
        f.readClause({ Lit(2, true), Lit(3) });
        Trail trail {};
        CHECK(trail.init(f));
        CHECK(trail.push(Lit(1, false)));
        CHECK(trail.propagate());
        CHECK(trail[0] == Lit(1, false));
        CHECK(trail[1] == Lit(2, false));
        CHECK(trail[2] == Lit(3, false));
        CHECK(trail.backtrack());
        CHECK(trail.push(Lit(2, true)));
        CHECK(trail.propagate());
        CHECK(trail[0] == Lit(2, true));
        CHECK(trail[1] == Lit(1, true));
    }
}