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
#include <queue>
#include <chrono>

struct Pos {
    int64_t x;
    int64_t y;

    bool operator==(const Pos& other) const {
        return x == other.x && y == other.y;
    }

    std::string toString() {
        return std::to_string(x) + "," + std::to_string(y);
    }
};

struct PosHash {
    size_t operator()(const Pos& p) const noexcept {
        size_t h1 = std::hash<int64_t>{}(p.x);
        size_t h2 = std::hash<int64_t>{}(p.y);
        return h1 ^ (h2 + 0x9e3779b97f4a7c15ULL + (h1 << 6) + (h1 >> 2));
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

int64_t solve() {
    std::cout << "parsing positions" << "\n";
    std::vector<Pos> redPositions = parsePositions();
    int nRed = redPositions.size();

    auto start = std::chrono::high_resolution_clock::now();
    
    //
    // Calculate border positions
    //
    std::cout << "calculating border positions" << "\n";
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

    for (auto &p : borderPositions) {
        auto &row = p.second;
        std::sort(row.begin(), row.end());
    }

    //
    // Calculate zone via row-wise scan
    //
    std::cout << "calculating zone via row-wise scan" << "\n";
    std::unordered_map<int64_t, std::vector<std::pair<int64_t, int64_t>>> zoneRowRanges;
    for (auto &p : borderPositions) {
        auto rowNum = p.first;
        auto &row = p.second;
        int n = row.size();
        int64_t i = 0;
        while (i < n) {
            int64_t low, high;

            // walk first set
            int64_t j = i;
            low = row[j];
            while (j < n - 1 && row[j] + 1 == row[j+1]) {
                j++;
            }
            if (j == n - 1) {
                // only one set
                high = row[j];
                zoneRowRanges[rowNum].push_back({low, high});
                break;
            }

            // walk second set
            j++;
            while (j < n - 1 && row[j] + 1 == row[j+1]) {
                j++;
            }
            high = row[j];
            zoneRowRanges[rowNum].push_back({low, high});

            i = j + 1;
        }
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
    std::cout << "iterating all pairs" << "\n";
    int64_t maxArea = INT64_MIN;
    int64_t cnt = 0;
    for (int i = 0; i < nRed; i++) {
        for (int j = i + 1; j < nRed; j++) {
            Pos p1 = redPositions[i];
            Pos p2 = redPositions[j];

            // skip rectangle if its too small to beat current max
            int64_t potentialArea = (std::abs(p1.x - p2.x) + 1) * (std::abs(p1.y - p2.y) + 1);
            if (potentialArea <= maxArea) {
                std::cout << "skipping" << "\n";
                continue;
            }
            std::cout << cnt << ": " << p1.toString() << " " << p2.toString() << "\n";

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
                auto &ranges = zoneRowRanges[row];
                auto leftSideRange = findRangeOfEl(p1.x, ranges);
                auto rightSideRange = findRangeOfEl(p2.x, ranges);
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

            cnt++;
            if (!allRowsConnected) {
                continue;
            }

            // at this point - we know rectangle is valid
            int64_t area = (std::abs(p1.x - p2.x) + 1) * (std::abs(p1.y - p2.y) + 1);
            maxArea = std::max(maxArea, area);
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    auto durationMilli = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    std::cout << "time - " << durationMilli << " ms" << "\n";

    return maxArea;
}

int main() {
    int64_t res = solve();
    std::cout << res << "\n";
    return 0;
}