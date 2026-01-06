#include <cstdint>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <cassert>
#include <cmath>
#include <sstream>

struct Pos {
    int64_t x;
    int64_t y;
    
    std::string toString() {
        return std::to_string(x) + "," + std::to_string(y);
    }
};

std::vector<Pos> parsePositions() {
    std::string filename = "positions.txt";
    std::fstream f(filename);
    if (!f.is_open()) {
        std::cout << "couldn't open file - " << filename << "\n";
        return {};
    }

    std::vector<Pos> positions;
    std::string line;
    int64_t x, y;
    char c;
    while (std::getline(f, line)) {
        std::stringstream ss(line);
        ss >> x >> c >> y;
        if (c != ',') {
            std::cout << "bad char - " << c << "\n";
            assert(false);
        }

        Pos pos = {x, y};
        positions.push_back(pos);
    }

    return positions;
}

int64_t solve() {
    //
    // naive
    //      - iterate each pair 
    //      - check each position in rectangle is 'in the zone', have an unordered_set of zone positions to make fast
    //      - probably too slow: (435^2) * average_enclosed_area == huge
    //
    // smarter
    //      - something dfs-like?
    //

    return 0;
}

int main() {
    int64_t res = solve();
    std::cout << res << "\n";
    return 0;
}