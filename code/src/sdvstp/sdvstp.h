
#pragma once

#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <cassert>

enum CellState { TO_BE_TILLED = '+', NOT_TO_BE_TILLED = '-', TILLED = 'T' };

// Template for any tilling pattern, simply defined as a w x h rectangle.
struct Pattern {
    int width; int height;
    void rotate() {std::swap(width, height);}
    Pattern getRotated() const {Pattern p = *this; p.rotate(); return p;}
    bool operator==(const Pattern& other) const {return width == other.width && height == other.height;}
    bool operator<(const Pattern& other) const {
        if (height < other.height) return true;
        if (height == other.height && width < other.width) return true;
        return false;
    }
};

// The five basic tilling patterns we are dealing with.
const Pattern P_BASE    {1, 1};
const Pattern P_COPPER  {1, 3};
const Pattern P_IRON    {1, 5};
const Pattern P_GOLD    {3, 3};
const Pattern P_IRIDIUM {3, 6};
const std::vector<Pattern> PATTERNS = {P_BASE, P_COPPER, P_IRON, P_GOLD, P_IRIDIUM};

// Template for a single action. "p" should be one of the five patterns above,
// then "rotate" indicates whether the pattern should still be rotated.
// (x,y) is the coordinate of the top left corner of the pattern (after rotating).
struct Action { Pattern p; bool rotate {false}; int x; int y; };

// Template for an action in the SDVSTPP framework (with player movement).
struct SDVSTPPAction {
    enum Type {MOVE, TILL_1, TILL_2, TILL_3, TILL_4, TILL_5} type;
    enum Direction {UP, DOWN, LEFT, RIGHT} dir;
};

// Class for the problem grid
struct Grid {
private:
    std::vector<std::vector<CellState>> states;
public:
    Grid(int w, int h) : states(h, std::vector<CellState>(w, TO_BE_TILLED)) {}
    int height() const {return states.size();}
    int width() const {assert(states.size() > 0); return states[0].size();}
    CellState at(int x, int y) const {return states[y][x];}
    CellState& at(int x, int y) {return states[y][x];}
    // Checks whether the provided action is applicable in the sense that it is entirely
    // contained in the grid.
    bool applicable(Action a) const {
        if (a.rotate) a.p.rotate(); // rotate pattern if necessary
        return a.x >= 0 && a.x + a.p.width <= width()
            && a.y >= 0 && a.y + a.p.height <= height();
    }
    // Apply the action, transforming all concerned cells into the provided state
    // (TILLED by default).
    void apply(Action a, CellState targetState = TILLED) {
        if (!applicable(a)) throw std::runtime_error("Action not applicable!");
        if (a.rotate) a.p.rotate(); // rotate pattern if necessary
        // Apply pattern to the grid
        for (int offsetX = 0; offsetX < a.p.width; offsetX++) {
            for (int offsetY = 0; offsetY < a.p.height; offsetY++) {
                at(a.x + offsetX, a.y + offsetY) = targetState;
            }
        }
    }
    // Apply the provided SDVSTPP action given the provided player position.
    // Returns the new player position.
    std::pair<int, int> apply(int posX, int posY, SDVSTPPAction a) {
        int destX = posX + (a.dir == SDVSTPPAction::RIGHT) - (a.dir == SDVSTPPAction::LEFT);
        int destY = posY + (a.dir == SDVSTPPAction::DOWN) - (a.dir == SDVSTPPAction::UP);
        if (a.type == SDVSTPPAction::MOVE) {
            return {destX, destY};
        }

        // We internally construct a basic (SDVSTP) tilling action and then apply it.

        Action tillAction;
        // Set the base pattern of the action.
        if (a.type == SDVSTPPAction::TILL_1) tillAction.p = P_BASE;
        if (a.type == SDVSTPPAction::TILL_2) tillAction.p = P_COPPER;
        if (a.type == SDVSTPPAction::TILL_3) tillAction.p = P_IRON;
        if (a.type == SDVSTPPAction::TILL_4) tillAction.p = P_GOLD;
        if (a.type == SDVSTPPAction::TILL_5) tillAction.p = P_IRIDIUM;
        // A horizontal orientation of the pattern means we have to rotate it explicitly.
        bool horizontal = a.dir == SDVSTPPAction::LEFT || a.dir == SDVSTPPAction::RIGHT;
        if (horizontal) tillAction.p.rotate();
        // Set the pattern's initial anchor position to the action's destination cell.
        tillAction.x = destX;
        tillAction.y = destY;
        // Move the anchor so that the pattern is at the correct location relative to the player.
        if (horizontal) {
            // If the pattern goes to the left, shift it to the left accordingly.
            if (a.dir == SDVSTPPAction::LEFT) tillAction.x -= tillAction.p.width - 1;
            // Center the pattern vertically at the destination position.
            tillAction.y -= (tillAction.p.height / 2);
        } else {
            // If the pattern goes to the top, shift it to the top accordingly.
            if (a.dir == SDVSTPPAction::UP) tillAction.y -= tillAction.p.height - 1;
            // Center the pattern horizontally at the destination position.
            tillAction.x -= (tillAction.p.width / 2);
        }
        // Apply the action, return the unchanged prior position of the player.
        apply(tillAction);
        return {posX, posY};
    }
    // Output the grid to the provided output stream (std::cout by default).
    // If "comment" is true, prepend "c " to each line and add a header also beginning with "c ".
    void print(std::ostream* osPtr = &std::cout, bool comment = true) const {
        auto& os = *osPtr;
        if (comment) os << "c Grid (" << width() << " x " << height() << "):" << std::endl;
        for (int y = 0; y < height(); y++) {
            if (comment) os << "c ";
            for (int x = 0; x < width(); x++) {
                os << std::string(1, at(x, y));
            }
            os << std::endl;
        }
    }

    // Parse a .sdvstp problem file and return the corresponding Grid instance.
    static Grid parse(const std::string& filename) {
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

    // Heuristic for generating random grids
    float tillableRatio() const {
        int tillable = 0;
        for (int y = 0; y < height(); y++) {
            for (int x = 0; x < width(); x++) {
                tillable += at(x, y) == TO_BE_TILLED;
            }
        }
        return tillable / (float) (width() * height());
    }
    // Generate a random grid. Used for benchmark generation.
    static Grid generateRandomGrid(int width, int height, float targetTillableRatio = 0.7, float minTillableRatio = 0.5) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> distCoin(0, 1);
        std::uniform_int_distribution<int> distPosX(0, width-1);
        std::uniform_int_distribution<int> distPosY(0, height-1);
        std::uniform_int_distribution<size_t> distSmallPattern(0, 1);

        Grid grid(width, height);
        int itCount = 0;
        while (itCount < 10'000 && grid.tillableRatio() > targetTillableRatio) {
            itCount++;
            // Select a random small pattern
            Pattern p = PATTERNS[distSmallPattern(gen)];
            Action a {p, distCoin(gen) > 0, distPosX(gen), distPosY(gen)};
            if (!grid.applicable(a)) continue;
            Grid copy = grid;
            copy.apply(a, distCoin(gen) ? TILLED : NOT_TO_BE_TILLED);
            if (copy.tillableRatio() < minTillableRatio) continue;
            grid = copy;
        }
        return grid;
    }

    // Generate a set of benchmarks to the specified output path.
    // Not necessarily the (only) set of benchmarks we evaluate the solvers with.
    static void generateBenchmarks(const std::string& outPath) {
        for (int width = 3; width <= 32; width = std::max((int) (1.2*width), width+1)) {
            for (int height = 3; height <= width; height = std::max((int) (1.3*height), height+1)) {
                for (int rep = 1; rep <= 5; rep++) {
                    Grid grid = generateRandomGrid(width, height);
                    auto outFile = outPath + "/gen-w" + std::to_string(width) + "-h"
                        + std::to_string(height) + "-rep" + std::to_string(rep) + ".sdvstp";
                    std::ofstream ofs(outFile);
                    ofs << "p sdvstp " << width << " " << height << std::endl;
                    grid.print(&ofs, false);
                    ofs.close();
                    std::cout << "(" << width << ", " << height << ", " << rep << ") : written to " << outFile << std::endl;
                }            
            }   
        }
    }
};
