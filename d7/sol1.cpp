#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

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

int solve() {
    auto grid = parseDiagram();
    int rows = grid.size();
    int cols = grid[0].size();

    // find S position, and place first beam
    int startCol = -1;
    for (int i = 0; i < cols; i++) {
        if (grid[0][i] == START) {
            startCol = i;
        }
    }
    if (startCol == -1) {
        std::cout << "start pos not found" << "\n";
        return -1;
    }
    grid[1][startCol] = BEAM;

    int splitCnt = 0;
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
                splitCnt++;
            } else if (grid[i][j] == EMPTY) {
                // propogate beam
                grid[i][j] = BEAM;
            } else if (grid[i][j] == BEAM) {
                // ok
            }
        }
    }
    std::cout << gridToString(grid);
    return splitCnt;
}

int main() {
    int res = solve();
    // std::cout << res << "\n";
    return 0;
}