#include <cstdint>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <cassert>
#include <cmath>
#include <sstream>
#include <unordered_set>

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

    /**
     * Zone defined as follows:
     *      - rows of ranges that are occupied, e.g. row 0 - {[0-2],[5-8]}, row 1 - {[4-6],[9-15]}, ...
     *      
     * Steps:
     *      maxArea = -1
     *      for each pair of red tiles
     *          for each row they enclose
     *              if leftmost col and rightmost col in different occupied sets => some part of row not in zone
     *                  break
     *          (rectangle entirely in zone at this point)
     *          maxArea = max(maxArea, areaOfCurrRectangle)
     *      return maxArea
     */

    return 0;
}

int main() {
    int64_t res = solve();
    std::cout << res << "\n";
    return 0;
}