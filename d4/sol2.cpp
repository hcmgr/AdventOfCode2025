#include <iostream>
#include <fstream>

#define ROLL '@'
#define EMPTY '.'

static std::vector<std::pair<int, int>> offs = {
    {0, 1},
    {1, 1},
    {1, 0},
    {1, -1},
    {0, -1},
    {-1, -1},
    {-1, 0},
    {-1, 1}
};

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

void dfs(int r, int c, std::vector<std::vector<char>> &grid, int m, int n) {
    if (grid[r][c] != ROLL) {
        return;
    }

    // get neighbour count
    int neiCount = 0;
    for (auto &p : offs) {
        int nr = r + p.first;
        int nc = c + p.second;
        if (nr >= 0 && nr < m && nc >= 0 && nc < n && grid[nr][nc] == ROLL) {
            neiCount += 1;
        }
    }

    if (neiCount >= 4) {
        return;
    }

    // can be removed - remove the roll, and update neighbours
    grid[r][c] = EMPTY;
    for (auto &p : offs) {
        int nr = r + p.first;
        int nc = c + p.second;
        if (nr >= 0 && nr < m && nc >= 0 && nc < n && grid[nr][nc] == ROLL) {
            dfs(nr, nc, grid, m, n);
        }
    }
}

int solve() {
    auto grid = parseRollGrid();
    int m = grid.size();
    int n = grid[0].size();

    // get initial roll count
    int initRollCnt = 0;
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) {
            if (grid[r][c] == ROLL) {
                initRollCnt++;
            }
        }
    }

    // remove all possible rolls
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) {
            dfs(r, c, grid, m, n);
        }
    }

    // get final roll count
    int finalRollCnt = 0;
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) {
            if (grid[r][c] == ROLL) {
                finalRollCnt++;
            }
            
        }
    }

    return initRollCnt - finalRollCnt;
}

int main() {
    int res = solve();
    std::cout << res << "\n";
    return 0;
}