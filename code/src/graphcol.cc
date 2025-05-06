
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <utility>

std::vector<std::pair<int, int>> parseFile(const std::string& filename) {
    std::vector<std::pair<int, int>> edgePairs;
    std::ifstream file(filename);
    std::string line;

    if (!file) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return edgePairs;
    }

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        char identifier;
        int first, second;

        if (iss >> identifier >> first >> second && identifier == 'e') {
            edgePairs.emplace_back(first, second);
        }
    }

    return edgePairs;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    std::vector<std::pair<int, int>> edges = parseFile(filename);

    // Display parsed edges
    for (const auto& edge : edges) {
        std::cout << "(" << edge.first << ", " << edge.second << ")" << std::endl;
    }

    // TODO: Implement a SAT-based graph colorer
    // TODO: Output a line like the following, where X is the found number of colors:
    // s SOLUTION X

    return 0;
}
