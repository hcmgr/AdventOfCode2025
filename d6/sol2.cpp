#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cmath>
#include <cassert>

enum Op {
    ADD,
    MUL
};

struct Problem {
    std::vector<int> nums;
    Op op;
};

std::vector<Problem> parseProblems() {
    std::string filename = "problems.txt";
    std::fstream f(filename);
    if (!f.is_open()) {
        std::cout << "couldn't open file - " << filename << "\n";
        return {};
    }

    std::vector<std::vector<int>> lines;
    std::vector<Op> ops;

    std::string line;
    int lineCnt = 0;
    while (std::getline(f, line)) {
        if (lineCnt < 4) {
            std::istringstream iss(line);
            int n;
            std::vector<int> nums;
            while (iss >> n) {
                nums.push_back(n);
            }
            lines.push_back(nums);
        } else {
            std::istringstream iss(line);
            char op;
            while (iss >> op) {
                if (op == '+') {
                    ops.push_back(ADD);
                } else if (op == '*') {
                    ops.push_back(MUL);
                } else {
                    throw std::runtime_error("unknown op - " + std::to_string(op));
                }
            }
        }
        lineCnt++;
    }

    int n = lines[0].size();
    std::vector<Problem> problems(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < 4; j++) {
            problems[i].nums.push_back(lines[j][i]);
        }
        problems[i].op = ops[i];
    }

    return problems;
}

int numDigits(int n) {
    int digits = 0;
    while (n > 0) {
        n /= 10;
        digits += 1;
    }
    return digits;
}

int ithDigit(int n, int i) {
    while (i > 0) {
        n /= 10;
        i--;
        if (n == 0) {
            return -1;
        }
    }
    return n % 10;
}

//
// Convert `problems` from column representation to ceph-representation (i.e. right-to-left, column-wise)
//
void convertProblemsToCephRep(std::vector<Problem> &problems) {
    for (auto &p: problems) {
        std::vector<int> &nums = p.nums;

        // find max num digits in column
        int maxDigits = INT_MIN;
        for (auto &num: nums) {
            maxDigits = std::max(maxDigits, numDigits(num));
        }

        //
        // for each digit column, build up numbers down the rows
        //
        std::vector<int> colNums;
        int nRows = p.nums.size();
        for (int c = 0; c < maxDigits; c++) {
            int colNum = 0;
            int nDigits = 0;
            for (int r = 0; r < nRows; r++) {
                int digit = ithDigit(nums[r], c);
                if (digit != -1) {
                    colNum += (digit * pow(10, nDigits));
                    nDigits++;
                }
            }
            colNums.push_back(colNum);
        }
        assert(colNums.size() == maxDigits);

        // for this problem, replace column-rep-nums with ceph-rep-nums
        p.nums = colNums;
    }
}

int64_t solve() {
    std::vector<Problem> problems = parseProblems();
    convertProblemsToCephRep(problems);

    int64_t total = 0;
    for (auto &p : problems) {
        
        Op op = p.op;
        int64_t res = op == ADD ? 0 : 1;
        for (auto &n : p.nums) {
            if (p.op == ADD) {
                res += n;
            } else {
                res *= n;
            }
        }
        total += res;
    }

    return total;
}

void testithDigit();

int main() {
    // int64_t res = solve();
    // std::cout << res << "\n";
    testithDigit();
    return 0;
}

void testithDigit() {
    int n, res;
    n = 12345;
    res = ithDigit(n, 0);
    assert(res == 5);
    res = ithDigit(n, 1);
    assert(res == 4);
    res = ithDigit(n, 4);
    assert(res == 1);
    res = ithDigit(n, 5);
    assert(res == -1);
}
