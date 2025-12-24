#include <vector>
#include <fstream>
#include <iostream>

enum Direction {
    Left,
    Right
};

struct Rotation {
    Direction direction;
    int positions;

    std::string toString() {
        std::string d = direction == Left ? "L" : "R";
        return d + std::to_string(positions);
    }
};

std::vector<Rotation> parseRotations() {
    std::vector<Rotation> rotations;

    std::string filename = "rots.txt";
    std::ifstream f(filename);
    if (!f.is_open()) {
        std::cout << "issue opening rots file - " << filename << "\n";
        return rotations;
    }

    char d;
    int p;
    while (f >> d >> p) {
        Direction direction;
        int pos;

        if (d == 'L') {
            direction = Left;
        } else {
            direction = Right;
        }
        pos = p;

        Rotation rotation = {direction, pos};
        rotations.push_back(rotation);
    }

    return rotations;
}

int mod(int k, int n) {
    return ((k % n) + n) % n;
}

int solve() {
    
    std::vector<Rotation> rotations = parseRotations();

    int pos = 50;
    int cnt = 0;
    for (auto &r : rotations) {
        Direction direction = r.direction;
        int full = r.positions / 100;
        int rem = mod(r.positions, 100);
        int newPos;

        cnt += full;
        if (rem == 0) {
            continue;
        }
        if (direction == Left) {
            newPos = mod((pos - rem), 100);
            if (newPos == 0 || (pos != 0 && newPos > pos)) {
                cnt += 1;
            }
        } else {
            newPos = mod((pos + rem), 100);
            if (newPos == 0 || (pos != 0 && newPos < pos)) {
                cnt += 1;
            }
        }
        std::cout << pos << "    " << r.toString() << "    " << newPos << "    " << cnt << "    " << full << "\n";
        pos = newPos;
    }
    return cnt;
}

int main() {
    int res = solve();
    std::cout << res << "\n";
    return 0;
}