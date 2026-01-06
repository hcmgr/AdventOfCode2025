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
    // trying to maximise dr*dc across all pairs of positions
    //
    std::vector<Pos> positions = parsePositions();
    for (auto &pos : positions) {
        std::cout << pos.toString() << "\n";
    }

    int64_t maxArea = INT64_MIN;
    std::vector<int64_t> areas;
    int n = positions.size();
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            Pos &p1 = positions[i];
            Pos &p2 = positions[j];
            int64_t dx = std::abs(p1.x - p2.x) + 1;
            int64_t dy = std::abs(p1.y - p2.y) + 1;
            maxArea = std::max(maxArea, dx * dy);
            areas.push_back(dx * dy);
        }
    }

    int64_t sumAreas = 0;
    for (auto &a : areas) sumAreas += a;
    int64_t avgAreas = sumAreas / areas.size();
    std::cout << avgAreas << "\n";

    return maxArea;
}

int main() {
    int64_t res = solve();
    std::cout << res << "\n";
    return 0;
}