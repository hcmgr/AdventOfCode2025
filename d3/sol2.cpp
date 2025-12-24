#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cassert>

std::vector<std::vector<int>> parseBanks() {
    std::string filename = "banks.txt";
    std::ifstream f(filename);
    if (!f.is_open()) {
        std::cout << "couldn't open file - " << filename << "\n";
        return {};
    }

    std::vector<std::vector<int>> banks;
    std::string line;
    while (std::getline(f, line)) {
        std::vector<int> bank;
        for (char &c : line) {
            bank.push_back(c - '0');
        }
        banks.push_back(bank);
    }

    return banks;
}

int64_t combineToDigit(std::vector<int>& digits) {
    int n = digits.size();
    int64_t num = 0;
    for (int i = 0; i < n; i++) {
        int64_t digit = digits[i];
        num += (digit * pow(10, n - 1 - i));
    }
    return num;
}

int64_t solve() {
    std::vector<std::vector<int>> banks = parseBanks();
    int64_t total = 0;
    auto start =  std::chrono::high_resolution_clock::now();

    for (auto &bank : banks) {
        int n = bank.size();
        std::vector<int> digits;

        // find our max 12-digit combination
        int max = -1;
        int maxInd = -1;
        for (int i = 0; i < 12; i++) {
            int start, end;
            if (i == 0) {
                start = 0;
            } else {
                start = maxInd + 1;
            }
            end = n - 11 + i;

            for (int j = start; j < end; j++) {
                if (bank[j] > max) {
                    max = bank[j];
                    maxInd = j;
                }
            }
            digits.push_back(max);
            max = -1; // reset max for this run
        }

        // create 12-digit number and add to count
        int64_t num = combineToDigit(digits);
        total += num;
    }

    auto end =  std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> elapsed = end - start;
    auto elapsed_micro = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    std::cout << "time elapsed - " << elapsed_micro << "us" << "\n";

    return total;
}

void testCombineToDigit();

int main() {
    int64_t res = solve();
    std::cout << res << "\n";
    return 0;
}

void testCombineToDigit() {
    std::vector<std::vector<int>> ds = {
        {1},
        {9,8},
        {1,2,3,4,5},
        {9,8,7,6,5,4,3,2,1,1,1,1}
    };
    std::vector<int64_t> expected = {
        1, 
        98,
        12345,
        987654321111
    };

    int n = ds.size();
    for (int i = 0; i < n; i++) {
        assert(combineToDigit(ds[i]) == expected[i]);
    }
}