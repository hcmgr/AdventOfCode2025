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
// Given series of non-overlapping ranges `ranges`, returns index of the range that `el` lies in.
// Returns -1 if `el` sits in none of the ranges.
//
int findRangeOfEl(int64_t el, std::vector<std::pair<int64_t, int64_t>> &ranges) {
    int n = ranges.size();
    for (int i = 0; i < n; i++) {
        auto r = ranges[i];
        if (r.first <= el && el <= r.second) {
            return i;
        }
    }
    return -1;
}

//
// From sorted list `els`, compute ranges of contiguous elements.
//
// For example:
//      {1,2,4,5,6,9,10,11,13} => {[1,2], [4,6], [9,11], [13,13]}
//
std::vector<std::pair<int64_t, int64_t>> computeRanges(std::vector<int64_t> &els) {
    std::vector<std::pair<int64_t, int64_t>> res;
    int n = els.size();
    int i = 0;
    while (i < n) {
        int64_t currMin = els[i];
        while (i + 1 < n && els[i+1] == els[i] + 1) {
            i++;
        }
        res.push_back({currMin, els[i]});
        i++;
    }
    return res;
}

int64_t solve() {
    std::vector<Pos> redPositions = parsePositions();
    int nRed = redPositions.size();

    //
    // Calculate border positions - mapping of form: row_num -> {posi, posj, ...}, each row sorted.
    //
    std::unordered_map<int64_t, std::vector<int64_t>> borderPositions;
    auto fillBetween = [&](Pos& prev, Pos& curr) {
        if (prev.x == curr.x) {
            // vertical case
            int dy = curr.y > prev.y ? 1 : -1;
            int64_t x = prev.x;
            int64_t y = prev.y + dy;
            while (y != curr.y) {
                borderPositions[y].push_back(x);
                y += dy;
            }
        } 
        else if (prev.y == curr.y) {
            // horizontal case
            int dx = curr.x > prev.x ? 1 : -1;
            int64_t x = prev.x + dx;
            int64_t y = prev.y;
            while (x != curr.x) {
                borderPositions[y].push_back(x);
                x += dx;
            }
        } 
        else {
            std::cout << "bad position pair" << "\n";
            assert(false);
        }
    };

    Pos initial = redPositions[0];
    borderPositions[initial.y].push_back(initial.x);
    for (int i = 1; i < nRed; i++) {
        Pos& prev = redPositions[i - 1];
        Pos& curr = redPositions[i];
        borderPositions[curr.y].push_back(curr.x);
        fillBetween(prev, curr);

        if (i == nRed - 1) {
            // at end - close loop
            fillBetween(curr, initial);
        }
    }

    // ensure rows are sorted
    for (auto &p : borderPositions) {
        auto &row = p.second;
        std::sort(row.begin(), row.end());
    }

    //
    // Build up the zone map. 
    //
    // Mapping of the form: row_num -> {range0, range1, ...}, where rangei is a range of 
    // covered positions in that row, e.g. [0, 2] => positions 0,1,2 all in zone.
    //
    std::unordered_map<int64_t, std::vector<std::pair<int64_t, int64_t>>> zone;
    for (auto &p : borderPositions) {
        auto rowNum = p.first;
        auto &row = p.second;

        // TODO
    }

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
                auto leftSideRange = findRangeOfEl(p1.x, range);
                auto rightSideRange = findRangeOfEl(p2.x, range);
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

void testComputeRanges();

int main() {
    int64_t res = solve();
    std::cout << res << "\n";
    return 0;
}

/////////////////////////////////
// Testing
/////////////////////////////////

void testComputeRanges() {
    std::vector<int64_t> els = {1,2,4,5,6,9,10,11,13,14,15,16,18};
    auto ranges = computeRanges(els);
    for (auto &r : ranges) {
        std::cout << r.first << "-" << r.second << "\n";
    }
}