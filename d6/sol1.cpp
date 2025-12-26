#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

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

int64_t solve() {
    std::vector<Problem> problems = parseProblems();
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

int main() {
    int64_t res = solve();
    std::cout << res << "\n";
    return 0;
}
