#include <iostream>
#include <fstream>

#define ROLL '@'
#define EMPTY '.'

std::vector<std::vector<char>> parseRollGrid() {
    std::string filename = "rolls.txt";
    std::fstream f(filename);
    if (!f.is_open()) {
        std::cout << "couldn't open file - " << filename << "\n";
        return {};
    }

    std::vector<std::vector<char>> grid;
    std::string line;
    while (std::getline(f, line)) {
        std::vector<char> row;
        for (auto &c : line) {
            row.push_back(c);
        }
        grid.push_back(row);
    }

    return grid;
}

int solve() {
    std::vector<std::pair<int, int>> offs = {
        {0, 1},
        {1, 1},
        {1, 0},
        {1, -1},
        {0, -1},
        {-1, -1},
        {-1, 0},
        {-1, 1}
    };

    int cnt = 0;
    auto grid = parseRollGrid();
    int m = grid.size();
    int n = grid[0].size();
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) {
            if (grid[r][c] != ROLL) {
                continue;
            }

            int neiRolls = 0;
            for (auto &p : offs) {
                int nr = r + p.first;
                int nc = c + p.second;
                if (nr >= 0 && nr < m && nc >= 0 && nc < n && grid[nr][nc] == ROLL) {
                    neiRolls += 1;
                }
            }
            if (neiRolls < 4) {
                cnt += 1;
            }
        }
    }

    return cnt;
}

int main() {
    int res = solve();
    std::cout << res << "\n";
    return 0;
}