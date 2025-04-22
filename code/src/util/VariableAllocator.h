/**
 * @brief Simple class to allocate variables in a structured way.
 * @author Markus Iser
 * @license MIT
 */

class VariableAllocator {
    unsigned next = 1;

public:
    unsigned allocate() {
        return next++;
    }

    unsigned* allocate(unsigned n) {
        unsigned* result = new unsigned[n];
        for (unsigned i = 0; i < n; i++) {
            result[i] = allocate();
        }
        return result;
    }

    unsigned** allocate(unsigned n, unsigned m) {
        unsigned** result = new unsigned*[n];
        for (unsigned i = 0; i < n; i++) {
            result[i] = new unsigned[m];
            for (unsigned j = 0; j < m; j++) {
                result[i][j] = allocate();
            }
        }
        return result;
    }

    unsigned*** allocate(unsigned n, unsigned m, unsigned o) {
        unsigned*** result = new unsigned**[n];
        for (unsigned i = 0; i < n; i++) {
            result[i] = new unsigned*[m];
            for (unsigned j = 0; j < m; j++) {
                result[i][j] = new unsigned[o];
                for (unsigned k = 0; k < o; k++) {
                    result[i][j][k] = allocate();
                }
            }
        }
        return result;
    }

};