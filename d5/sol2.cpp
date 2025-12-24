#include <cstdint>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <chrono>
#include <cmath>

struct Range {
    int64_t low;
    int64_t high;

    std::string toString() {
        return std::to_string(low) + "-" + std::to_string(high);
    }
};

struct Input {
    std::vector<Range> ranges;
    std::vector<int64_t> ids;
};

Input parseInput() {
    std::string filename = "ids.txt";
    std::fstream f(filename);
    if (!f.is_open()) {
        std::cout << "couldn't open file - " << filename << "\n";
        return {};
    }

    // parse ranges
    std::vector<Range> ranges;
    std::string line;
    while (std::getline(f, line)) {
        if (line == "") {
            break;
        }

        auto dashPos = line.find('-');
        if (dashPos == std::string::npos) {
            return {};
        }

        int64_t low = std::stoll(line.substr(0, dashPos));
        int64_t high = std::stoll(line.substr(dashPos + 1));
        Range range = {low, high};
        ranges.push_back(range);
    }

    // parse ids
    std::vector<int64_t> ids;
    while (std::getline(f, line)) {
        int64_t id = std::stoll(line);
        ids.push_back(id);
    }

    Input input = {ranges, ids};
    return input;
}

int64_t solve() {
    Input input = parseInput();
    std::vector<Range> &ranges = input.ranges;

    // sort by low
    auto start = std::chrono::high_resolution_clock::now();
    std::sort(ranges.begin(), ranges.end(), [](Range &r1, Range &r2) {
        return r1.low < r2.low;
    });

    //
    // merge
    //
    // l1 h1 l2 h2 -> (l1 h1) (l2 h2)
    // l1 l2 h1 h2 -> (l1, h2)
    // 
    int n = ranges.size();
    std::vector<Range> mergedRanges;
    mergedRanges.push_back(ranges[0]);
    for (int i = 1; i < n; i++) {
        Range &r1 = mergedRanges.back();
        Range &r2 = ranges[i];
        if (r2.low <= r1.high) {
            r1.high = std::max(r1.high, r2.high);
        } else {
            mergedRanges.push_back(r2);
        }
    }

    int64_t cnt = 0;
    for (auto &r : mergedRanges) {
        cnt += (r.high - r.low + 1);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> elapsed = end - start;
    auto elapsedMicro = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    std::cout << "time - " << elapsedMicro << "us" << "\n";

    return cnt;
}

int main() {
    int64_t res = solve();
    std::cout << res << "\n";
    return 0;
}