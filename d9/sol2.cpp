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

    //
    // get list of border points
    // line by line, add positions (or add ranges of positions)
    // for each pair
    //      for each row enclosed
    //          if far left and far right are in distinct sets
    //              break
    //      (here, we know its valid rectangle inside zone)
    //      maxArea = max(maxArea, areaOfCurrRectangle)
    // return maxArea
    //

    return 0;
}

int main() {
    int64_t res = solve();
    std::cout << res << "\n";
    return 0;
}