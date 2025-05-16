
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
#include <vector>

enum CellState { TO_BE_TILLED = '+', NOT_TO_BE_TILLED = '-', TILLED = 'T' };

// Template for any tilling pattern, simply defined as a w x h rectangle.
struct Pattern {
    int width; int height;
    void rotate() {std::swap(width, height);}
    bool operator==(const Pattern& other) const {return width == other.width && height == other.height;}
};

// The five basic tilling patterns we are dealing with.
const Pattern P_BASE    {1, 1};
const Pattern P_COPPER  {1, 3};
const Pattern P_IRON    {1, 5};
const Pattern P_GOLD    {3, 3};
const Pattern P_IRIDIUM {3, 6};

// Template for a single action. "p" should be one of the five patterns above,
// then "rotate" indicates whether the pattern should still be rotated.
struct Action { Pattern p; bool rotate; int x; int y; };

// Class for the problem grid
struct Grid {
private:
    std::vector<std::vector<CellState>> states;
public:
    Grid(int w, int h) : states(h, std::vector<CellState>(w)) {}
    int height() const {return states.size();}
    int width() const {assert(states.size() > 0); return states[0].size();}
    CellState at(int x, int y) const {return states[y][x];}
    CellState& at(int x, int y) {return states[y][x];}
    bool applicable(Action a) const {
        if (a.rotate) a.p.rotate(); // rotate pattern if necessary
        return a.x + a.p.width < width()
            && a.y + a.p.height < height();
    }
    void apply(Action a) {
        if (!applicable(a)) throw std::runtime_error("Action not applicable!");
        if (a.rotate) a.p.rotate(); // rotate pattern if necessary
        // Apply pattern to the grid
        for (int offsetX = 0; offsetX < a.p.width; offsetX++) {
            for (int offsetY = 0; offsetY < a.p.height; offsetY++) {
                at(a.x + offsetX, a.y + offsetY) = TILLED;
            }
        }
    }
    void print() const {
        std::cout << "c Grid (" << width() << " x " << height() << "):" << std::endl;
        for (int y = 0; y < height(); y++) {
            std::cout << "c ";
            for (int x = 0; x < width(); x++) {
                std::cout << std::string(1, at(x, y));
            }
            std::cout << std::endl;
        }
    }
};

// Parse a .sdvstp problem file and return the corresponding Grid instance.
Grid parseSDVSTPFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    std::string header;
    int width, height;

    // Read and parse the header line
    std::getline(file, header);
    std::istringstream headerStream(header);
    std::string format;
    headerStream >> format >> format >> width >> height;

    if (format != "sdvstp") {
        throw std::runtime_error("Invalid file format");
    }

    // Read grid data
    Grid grid(width, height);
    for (int y = 0; y < height; ++y) {
        std::string line;
        std::getline(file, line);
        if (line.size() != static_cast<size_t>(width)) {
            throw std::runtime_error("Unexpected line length in grid");
        }

        for (int x = 0; x < width; ++x) {
            switch (line[x]) {
                case '+': grid.at(x, y) = CellState::TO_BE_TILLED; break;
                case '-': grid.at(x, y) = CellState::NOT_TO_BE_TILLED; break;
                case 'T': grid.at(x, y) = CellState::TILLED; break;
                default:
                    throw std::runtime_error("Unexpected character in grid: " + std::string(1, line[x]));
            }
        }
    }

    return grid;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    const std::string filename = argv[1];
    const auto grid = parseSDVSTPFile(filename);

    // Display grid
    grid.print();

    // TODO: Implement a SAT-based SDVSTP solver to retrieve a solution like this.
    // Each item in the solution vector must be one of the objects P_*.
    std::vector<Action> solution = {
        {P_GOLD,   false, 0, 0},
        {P_GOLD,   false, 5, 1},
        {P_IRON,   true,  3, 0},
        {P_COPPER, true,  3, 1},
        {P_BASE,   false, 0, 3},
        {P_BASE,   0,     7, 4}
    };

    // Obtain the grid resulting from applying all actions.
    auto vGrid = grid;
    for (auto a : solution) {
        vGrid.apply(a);
    }
    vGrid.print();

    // Validate the soundness (not optimality!) of the solution.
    for (int y = 0; y < grid.height(); y++) {
        for (int x = 0; x < grid.width(); x++) {
            if (grid.at(x, y) == NOT_TO_BE_TILLED && vGrid.at(x, y) == TILLED) {
                throw std::runtime_error("Cell (" + std::to_string(x) + "," + std::to_string(y)
                    + ") was tilled but should not be!");
            }
            if (grid.at(x, y) == TO_BE_TILLED && vGrid.at(x, y) != TILLED) {
                throw std::runtime_error("Cell (" + std::to_string(x) + "," + std::to_string(y)
                    + ") was not tilled but should be!");
            }
        }   
    }
    std::cout << "Solution passed soundness check." << std::endl;

    // Output the solution.
    std::cout << "s SOLUTION " << solution.size() << std::endl;
    for (auto a : solution) {
        if (a.p == P_BASE)    std::cout << "base ";
        if (a.p == P_COPPER)  std::cout << "copper ";
        if (a.p == P_IRON)    std::cout << "iron ";
        if (a.p == P_GOLD)    std::cout << "gold ";
        if (a.p == P_IRIDIUM) std::cout << "iridium ";
        std::cout << (a.rotate ? "90" : "0") << " " << a.x << " " << a.y << std::endl;
    }
}
