#include <vector>
#include <cstdint>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <queue>
#include <cmath>
#include <unordered_set>
#include <cassert>
#include <memory>

#define LIMIT 1000

struct Pos {
    int id;
    int64_t x;
    int64_t y;
    int64_t z;

    std::string toString() const {
        std::ostringstream oss;
        oss << "(" << x << " " << y << " " << z << ")";
        return oss.str();
    }

    bool operator<(const Pos& other) const noexcept { return id < other.id; }
    bool operator==(const Pos& other) const noexcept { return id == other.id; }
};

struct PosHash {
    size_t operator()(const Pos& p) const noexcept {
        return std::hash<int>{}(p.id);
    }
};

using PosSet = std::unordered_set<Pos, PosHash>;
using PosPair = std::pair<Pos, Pos>;

std::string graphToString(
    const std::unordered_map<int, std::shared_ptr<PosSet>>& graph
) {
    std::ostringstream oss;
    oss << "{ ";

    bool firstSet = true;
    for (const auto& [setId, posSetPtr] : graph) {
        if (!firstSet) oss << ", ";
        firstSet = false;

        oss << setId << ":[";

        if (posSetPtr) {
            bool firstPos = true;
            for (const auto& pos : *posSetPtr) {
                if (!firstPos) oss << ", ";
                firstPos = false;
                oss << pos.toString();
            }
        } else {
            oss << "null";
        }

        oss << "]";
    }

    oss << " }";
    return oss.str();
}

std::vector<Pos> parsePositions() {
    std::string filename = "positions.txt";
    std::fstream f(filename);
    if (!f.is_open()) {
        std::cout << "couldn't open file - " << filename << "\n";
        return {};
    }

    std::vector<Pos> positions;
    std::string line;
    int64_t x, y, z;
    char c, d;
    int cnt = 0;
    while (std::getline(f, line)) {
        std::stringstream ss(line);
        ss >> x >> c >> y >> d >> z;
        if (!(x >= 0 && y >= 0 && z >= 0 && c == ',' && d == ',')) {
            std::cout << "problem parsing - " << cnt << "\n";
            return {};
        }

        Pos pos = {cnt, x, y, z};
        positions.push_back(pos);
        cnt++;
    }

    return positions;
}

float eucDistance(Pos &p1, Pos &p2) {
    return std::sqrt(
        std::pow((p1.x - p2.x), 2) + 
        std::pow((p1.y - p2.y), 2) + 
        std::pow((p1.z - p2.z), 2)
    );
}

void mergeSets(PosSet &s1, PosSet &s2) {
    for (auto &el : s2) {
        s1.insert(el);
    }
}

int64_t solve() {
    auto positions = parsePositions();

    // build up pairs list, sorted by distance
    std::vector<std::pair<float, std::pair<Pos, Pos>>> distPairs;
    int n = positions.size();
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            Pos p1 = positions[i];
            Pos p2 = positions[j];
            float dist = eucDistance(p1, p2);
            distPairs.push_back({dist, {p1, p2}});
        }
    }
    std::sort(distPairs.begin(), distPairs.end());

    // build up graph of sets
    std::unordered_map<int, std::shared_ptr<PosSet>> graph;     // setId -> {posX, posY, ...}
    std::unordered_map<int, int> posSetMap;                     // posId -> setId 
    for (auto &p : distPairs) {
        auto pair = p.second;
        auto pos1 = pair.first;
        auto pos2 = pair.second;

        // Make setId the id of the first position inserted. Since posIds are unique,
        // this ensures setIds are also unique.
        int set1Id = pos1.id;
        int set2Id = pos2.id;

        auto s1 = std::make_shared<PosSet>();
        auto s2 = std::make_shared<PosSet>();
        s1->insert(pos1);
        s2->insert(pos2);
        graph[set1Id] = s1;
        graph[set2Id] = s2;

        posSetMap[pos1.id] = set1Id;
        posSetMap[pos2.id] = set2Id;
    }

    // iteratively connect pair by pair, until #sets == 1
    bool done = false;
    std::pair<Pos, Pos> lastPair;
    int cnt = 0;
    for (auto &p : distPairs) {
        // find the sets pos1 and pos2 are in
        auto pair = p.second;
        auto pos1 = pair.first;
        auto pos2 = pair.second;
        int pos1SetId = posSetMap[pos1.id];
        int pos2SetId = posSetMap[pos2.id];
        auto pos1Set = graph[pos1SetId];
        auto pos2Set = graph[pos2SetId];
        if (pos1Set == nullptr || pos2Set == nullptr) {
            std::cout << pos1SetId << " " << pos2SetId << "\n";
            assert(false);
        }

        // pos1 and pos2 in distinct sets - merge into pos1 set
        if (pos1Set != pos2Set) {
            mergeSets(*pos1Set, *pos2Set);
            graph.erase(pos2SetId);
            
            // remap all pos2 set entries to pos1 set
            for (auto &pos : *pos2Set) {
                posSetMap[pos.id] = pos1SetId;
            }
        }

        if (graph.size() == 1) {
            // fully merged
            lastPair = p.second;
            done = true;
            break;
        }
        cnt++;
    }

    if (!done) {
        assert(false);
    }

    std::cout << "Pairs tried - " << cnt << "\n";
    return lastPair.first.x * lastPair.second.x;
}

int main() {
    int64_t res = solve();
    std::cout << res << "\n";
    return 0;
}