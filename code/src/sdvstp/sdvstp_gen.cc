
#include <cassert>
#include <iostream>

// Helper classes and methods for SDVSTP. Look inside sdvstp.h for the entire interface.
#include "sdvstp.h"

int main(int argc, char* argv[]) {

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <path>" << std::endl;
        return 1;
    }
    const std::string outputPath = argv[1];

    // Perform benchmark generation
    Grid::generateBenchmarks(outputPath);
}
