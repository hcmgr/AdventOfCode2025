#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cassert>
#include <ostream>

struct Machine {
    std::vector<bool> lights;
    std::vector<std::vector<int>> buttons;
    std::vector<int> joltages;

    std::string toString() {
        std::ostringstream oss;

        // lights
        for (auto l : lights) {
            char c = l ? '#' : '.';
            oss << c;
        }
        oss << " ";

        // buttons
        for (auto &button : buttons) {
            oss << "( ";
            for (auto &b : button) {
                oss << b << " ";
            }
            oss << ")";
        }
        oss << " ";

        // joltages
        oss << "{ ";
        for (auto &j : joltages) {
            oss << j << " ";
        }
        oss << "}";

        return oss.str();
    }
};

std::vector<Machine> parseMachines() {
    std::string filename = "machines.txt";
    std::fstream f(filename);
    if (!f.is_open()) {
        std::cout << "couldn't open file - " << filename << "\n";
        return {};
    }

    std::vector<Machine> machines;
    std::string line;
    while (std::getline(f, line)) {
        std::stringstream ss(line);

        //
        // lights
        //
        std::vector<bool> lights;
        char c;

        // opening bracket
        ss >> c;
        if (c != '[') assert(false);

        // lights
        while (true) {
            ss >> c;
            if (c == ']') {
                break;
            } else {
                bool onOff = c == '#' ? true : false;
                lights.push_back(onOff);
            }
        }

        //
        // buttons
        //
        std::vector<std::vector<int>> buttons;
        int b;
        while (true) {
            std::vector<int> button;
            ss >> c;

            // opening bracket
            if (c == '(') {
                // do nothing - ok
            } else if (c == '{') {
                // start of joltage list
                break;
            } else {
                assert(false);
            }

            // numbers
            while (true) {
                ss >> b; // button
                ss >> c; // char
                button.push_back(b);

                if (c == ',') {
                    // do nothing - ok
                } else if (c == ')') {
                    // end of list
                    break;
                } else {
                    assert(false);
                }
                
            }

            buttons.push_back(button);
        }

        //
        // joltages - opening bracket already parsed
        //
        std::vector<int> joltages;
        int j;
        while (true) {

            // numbers
            ss >> j; // joltage
            ss >> c; // char
            joltages.push_back(j);

            if (c == ',') {
                // do nothing - ok
            } else if (c == '}') {
                // end of list
                break;
            } else {
                assert(false);
            }
            

            // should be end of line here
        }

        machines.push_back({lights, buttons, joltages});
    }

    return machines;
}

int64_t solve() {
    std::vector<Machine> machines = parseMachines();
    std::cout << machines.size() << "\n";
    for (auto &machine : machines) {
        std::cout << machine.toString() << "\n";
    }
    return 0;
}

int main() {
    int64_t res = solve();
    std::cout << res << "\n";
    return 0;
}