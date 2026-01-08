#include <cstdint>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <cassert>
#include <cmath>
#include <sstream>
#include <unordered_set>
#include <map>

struct Pos {
    int64_t x;
    int64_t y;

    std::string toString() {
        return std::to_string(x) + "," + std::to_string(y);
    }
};

std::vector<Pos> parsePositions() {
    std::string filename = "test.txt";
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

//
// Given series of un-overlapping ranges `ranges`, returns index of the range that `el` lies in.
// Returns -1 if `el` sits in none of the ranges.
//
int findRange(int64_t el, std::vector<std::pair<int64_t, int64_t>> &ranges) {
    int n = ranges.size();
    for (int i = 0; i < n; i++) {
        auto r = ranges[i];
        if (r.first <= el && el <= r.second) {
            return i;
        }
    }
    return -1;
}

int64_t solve() {
    std::vector<Pos> redPositions = parsePositions();
    int nRed = redPositions.size();

    //
    // Calculate border positions
    //
    std::vector<Pos> borderPositions;
    borderPositions.push_back(redPositions[0]);
    for (int i = 1; i < nRed; i++) {
        Pos& prev = redPositions[i - 1];
        Pos& curr = redPositions[i];
        borderPositions.push_back(curr);

        //
        // fill positions between `prev` and `curr`
        //
        if (prev.x == curr.x) {
            // vertical case
            int dy = curr.y > prev.y ? 1 : -1;
            int64_t x = prev.x;
            int64_t y = prev.y + dy;
            while (y != curr.y) {
                borderPositions.push_back({x, y});
                y += dy;
            }
        } 
        else if (prev.y == curr.y) {
            // horizontal case
            int dx = curr.x > prev.x ? 1 : -1;
            int64_t x = prev.x + dx;
            int64_t y = prev.y;
            while (x != curr.x) {
                borderPositions.push_back({x, y});
                x += dx;
            }
        } 
        else {
            std::cout << "bad position pair" << "\n";
            assert(false);
        }
    }

    for (auto &p : borderPositions) {
        std::cout << p.toString() << "\n";
    }

    return 0;

    //
    // Build up the zone map. 
    //
    // Mapping of the form: row_num -> {range0, range1, ...}, where rangeI is a range of 
    // covered positions in that row, e.g. [0, 2] => positions 0,1,2 all in zone.
    //
    std::unordered_map<int64_t, std::vector<std::pair<int64_t, int64_t>>> zone;

    //
    // Iterate all red pairs to find valid rectangles (and max area one).
    // 
    // Algorithm:
    //
    // for each pair of red positions
    //      for each row enclosed
    //          if far left and far right are in distinct sets => row not connected => rectangle not all in zone
    //              break
    //      (here, we know its valid rectangle inside zone)
    //      maxArea = max(maxArea, areaOfCurrRectangle)
    //
    int64_t maxArea = INT64_MIN;
    for (int i = 0; i < nRed; i++) {
        for (int j = i + 1; j < nRed; j++) {
            Pos p1 = redPositions[i];
            Pos p2 = redPositions[j];

            // make p1 the left position
            if (p1.x > p2.x) {
                std::swap(p1, p2);
            }

            //
            // check that all the rows p1-p2 enclose are connected
            //
            int64_t y = p1.y;
            int dy = p1.y < p2.y ? 1 : -1;
            auto isRowConnected = [&](int64_t row) {
                auto range = zone[row];
                auto leftSideRange = findRange(p1.x, range);
                auto rightSideRange = findRange(p2.x, range);
                return leftSideRange != -1 && rightSideRange != -1 && leftSideRange == rightSideRange;
            };

            bool allRowsConnected = true;
            while (y != p2.y) {
                if (!isRowConnected(y)) {
                    allRowsConnected = false;
                    break;
                }
                y += dy;
            }
            if (!isRowConnected(y)) { // make sure also check p2.y
                allRowsConnected = false;
            }

            if (!allRowsConnected) {
                continue;
            }

            // at this point - we know rectangle is valid
            int64_t area = (std::abs(p1.x - p2.x) + 1) * (std::abs(p1.y - p2.y) + 1);
            maxArea = std::max(maxArea, area);
        }
    }

    return maxArea;
}

int main() {
    int64_t res = solve();
    std::cout << res << "\n";
    return 0;
}