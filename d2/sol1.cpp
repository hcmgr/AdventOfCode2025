#include <vector>
#include <fstream>
#include <iostream>
#include <cstdint>
#include <utility>
#include <chrono>

struct Range {
    int64_t low;
    int64_t high;

    std::string toString() {
        return std::to_string(low) + "-" + std::to_string(high);
    }
};

std::vector<Range> parseRanges() {
    std::vector<Range> ranges;
    std::string filename = "ranges.txt";
    std::ifstream f(filename);
    if (!f.is_open()) {
        std::cout << "couldn't open file - " << filename << "\n";
        return ranges;
    }

    int64_t low, high;
    char comma, dash;
    while (f >> low >> dash >> high) {
        Range r = {low, high};
        ranges.push_back(r);
        f >> comma;
    }
    return ranges;
}

int numDigits(int64_t n) {
    int digits = 0;
    while (n) {
        n /= 10;
        digits++;
    }
    return digits;
}

std::pair<int64_t, int64_t> splitDigit(int64_t n, int nDigits) {
    int64_t low = 0;
    int64_t high = 0;
    int m = nDigits / 2;
    int digit;
    for (int i = 0; i < nDigits; i++) {
        digit = n % 10;
        n /= 10;
        if (i < m) {
            low += (digit * pow(10, i));
        } else {
            high += (digit * pow(10, i-m));
        }
    }
    return std::make_pair(low, high);
}

int64_t solve() {
    std::vector<Range> ranges = parseRanges();
    int64_t cnt = 0;
    auto start =  std::chrono::high_resolution_clock::now();
    for (auto &r : ranges) {
        int64_t low = r.low;
        int64_t high = r.high;
        for (int64_t i = low; i <= high; i++) {
            int nDigits = numDigits(i);
            if (nDigits % 2) {
                continue;
            }
            std::pair<int64_t, int64_t> p = splitDigit(i, nDigits);
            int64_t first = p.first;
            int64_t second = p.second;
            if (first == second) {
                cnt += i;
            }
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Time elapsed - " << duration.count() << "\n";

    return cnt;
}

int main() {
    int64_t res = solve();
    std::cout << res << "\n";
    return 0;
}