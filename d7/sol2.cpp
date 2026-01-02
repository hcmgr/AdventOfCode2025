#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <cstdint>

#define START_C         'S'
#define SPLITTER_C      '^'
#define BEAM_C          '|'
#define EMPTY_C         '.'

enum Pos {
    START,
    SPLITTER,
    BEAM,
    EMPTY
};

char posToChar(Pos pos) {
    switch (pos) {
        case START:
            return START_C;
        case SPLITTER:
            return SPLITTER_C;
        case BEAM:
            return BEAM_C;
        case EMPTY:
            return EMPTY_C;
        default:
            return 0;
    }
}

std::vector<std::vector<Pos>> parseDiagram() {
    std::string filename = "beams.txt";
    std::fstream f(filename);
    if (!f.is_open()) {
        std::cout << "couldn't open file - " << filename << "\n";
        return {};
    }

    std::vector<std::vector<Pos>> grid;
    std::string line;
    while (std::getline(f, line)) {
        std::vector<Pos> row;
        for (auto &c : line) {
            Pos p;
            switch (c) {
                case START_C:
                    p = START;
                    break;
                case SPLITTER_C:
                    p = SPLITTER;
                    break;
                case BEAM_C:
                    p = BEAM;
                    break;
                case EMPTY_C:
                    p = EMPTY;
                    break;
                default: {
                    std::cout << "unknown pos found - " << c << "\n";
                    return {};
                }
            };
            row.push_back(p);
        }
        grid.push_back(row);
    }

    return grid;
}

std::string gridToString(const std::vector<std::vector<Pos>> &grid) {
    std::ostringstream oss;
    for (auto &r : grid) {
        for (auto &pos : r) {
            oss << posToChar(pos);
        }
        oss << "\n";
    }
    return oss.str();
}

std::string dpToString(const std::vector<std::vector<int64_t>> &grid) {
    std::ostringstream oss;
    for (auto &r : grid) {
        for (auto &pos : r) {
            oss << pos << " ";
        }
        oss << "\n";
    }
    return oss.str();
}

void propogateClassicalBeam(std::vector<std::vector<Pos>>& grid, int &startCol) {
    int rows = grid.size();
    int cols = grid[0].size();

    //
    // find S position, and place first beam
    //
    for (int i = 0; i < cols; i++) {
        if (grid[0][i] == START) {
            startCol = i;
        }
    }
    if (startCol == -1) {
        std::cout << "start pos not found" << "\n";
        return;
    }
    grid[1][startCol] = BEAM;

    //
    // propogate beam classically through grid
    //
    for (int i = 2; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            // only update if pos above was a beam
            if (grid[i-1][j] != BEAM) {
                continue;
            }

            if (grid[i][j] == SPLITTER) {
                // split beam
                if (j > 0) {
                    grid[i][j-1] = BEAM;
                }
                if (j < cols - 1) {
                    grid[i][j+1] = BEAM;
                }
            } else if (grid[i][j] == EMPTY) {
                // continue beam
                grid[i][j] = BEAM;
            } else if (grid[i][j] == BEAM) {
                // ok
            }
        }
    }
}

int64_t solve() {
    auto grid = parseDiagram();
    int startCol = -1;
    propogateClassicalBeam(grid, startCol);
    if (startCol == -1) {
        return -1;
    }

    // bottom-up dp
    int rows = grid.size();
    int cols = grid[0].size();
    std::vector<std::vector<int64_t>> dp(rows+1, std::vector<int64_t>(cols, 0));

    // base-case - all 1's
    for (int i = 0; i < cols; i++) {
        dp[rows][i] = 1;
    }

    for (int i = rows - 1; i >= 2; i--) {
        for (int j = 0; j < cols; j++) {
            Pos &pos = grid[i][j];
            if (pos == BEAM) {
                dp[i][j] = dp[i+1][j];
            }
            else if (pos == SPLITTER) {
                dp[i][j] = 0;
                if (j > 0) {
                    // left child
                    dp[i][j] += dp[i+1][j-1];
                }
                if (j < cols - 1) {
                    // right child
                    dp[i][j] += dp[i+1][j+1];
                }
            } 
            else {
                // ignore
            }
        }
    }

    std::cout << dpToString(dp);

    // first splitter (row 2) should hold number of timelines
    return dp[2][startCol];
}

int main() {
    int64_t res = solve();
    std::cout << res << "\n";
    return 0;
}