#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

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

int solve() {
    std::vector<std::vector<int>> banks = parseBanks();
    int cnt = 0;
    for (auto &bank : banks) {
        int n = bank.size();

        // find max (earliest instance)
        int max = -1;
        int maxInd = -1;
        for (int i = 0; i < n - 1; i++) {
            if (bank[i] > max) {
                max = bank[i];
                maxInd = i;
            }
        }

        // find max after [0 -> n-1]'s max (earliest instance)
        int maxAfter = bank[maxInd + 1];
        int maxAfterInd = maxInd + 1;
        for (int i = maxInd + 2; i < n; i++) {
            if (bank[i] > maxAfter) {
                maxAfter = bank[i];
                maxAfterInd = i;
            }
        }

        // add 2-digit num to count
        int num = max*10 + maxAfter;
        cnt += num;

        std::cout << num << "\n";
    }

    return cnt;
}

int main() {
    int res = solve();
    std::cout << res << "\n";
    return 0;
}