#include <cstdint>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

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

int solve() {
    Input input = parseInput();
    return 0;
}

int main() {
    int res = solve();
    // std::cout << res << "\n";
    return 0;
}