#include <vector>
#include <fstream>
#include <iostream>
#include <cstdint>
#include <utility>
#include <chrono>
#include <cassert>
#include <sstream>
#include <unordered_set>

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

std::vector<int64_t> splitDigitMul(int64_t n, int nGroups, int nDigits) {
    std::vector<int64_t> res(nGroups, 0);
    int nGroupDigits = nDigits / nGroups;
    int digit;
    for (int i = 0; i < nGroups; i++) {
        int start = i * nGroupDigits;
        int end = start + nGroupDigits;
        int64_t &curr = res[i];
        for (int j = start; j < end; j++) {
            digit = n % 10;
            n /= 10;
            curr += (digit * pow(10, j - start));
        }
    }
    
    return res;
}

bool allEqual(const std::vector<int64_t> &v) {
    int n = v.size();
    bool equal = true;
    int64_t el = v.front();
    for (int i = 1; i < n; i++) {
        if (v[i] != el) {
            equal = false;
            break;
        }
    }
    return equal;
}

int64_t solve() {
    std::vector<Range> ranges = parseRanges();
    int64_t cnt = 0;
    auto start = std::chrono::high_resolution_clock::now();

    for (auto &r : ranges) {
        int64_t low = r.low;
        int64_t high = r.high;
        for (int64_t i = low; i <= high; i++) {
            int nDigits = numDigits(i);
            for (int nGroups = 2; nGroups <= nDigits; nGroups++) {
                if (nDigits % nGroups) {
                    continue;
                }
                std::vector<int64_t> group = splitDigitMul(i, nGroups, nDigits);
                if (allEqual(group)) {
                    cnt += i;
                    break;
                }
            }
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = end - start;
    auto elapsed_milli = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
    std::cout << "Time elapsed - " << elapsed_milli << "ms" << "\n";

    return cnt;
}

void testSplitDigitMul();
void testAllEqual();

int main() {
    int64_t res = solve();
    std::cout << res << "\n";
    return 0;
}

//////////////////////////////////////////
// Tests
//////////////////////////////////////////
 
void testSplitDigitMul() {
    std::vector<std::pair<int64_t, int>> nums = {
        {111111, 6},
        {121212, 3},
        {123123, 2},
        {98769876, 2},
        {98989898, 4},
        {9876598765, 2}
    };
    std::vector<std::vector<int64_t>> expectedRes = {
        {1,1,1,1,1,1},
        {12,12,12},
        {123,123},
        {9876,9876},
        {98,98,98,98},
        {98765,98765}
    };

    int n = nums.size();
    for (int i = 0; i < n; i++) {
        int64_t el = nums[i].first;
        int nSplit = nums[i].second;
        int nDigits = numDigits(el);

        auto res = splitDigitMul(el, nSplit, nDigits);
        assert(res == expectedRes[i]);
    }
}

void testAllEqual() {
    std::vector<std::vector<int64_t>> vecs = {
        {12, 12, 12},
        {123, 123, 123, 124},
        {98765, 98765, 98765}
    };
    std::vector<bool> expectedRes = {
        true,
        false,
        true
    };

    int n = vecs.size();
    for (int i = 0; i < n; i++) {
        assert(allEqual(vecs[i]) == expectedRes[i]);
    }
}