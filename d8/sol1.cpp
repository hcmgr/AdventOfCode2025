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
        oss << id << " : " << "(" << x << " " << y << " " << z << ")";
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

    //
    // Find closest 1000 pairs by building up max heap of positions, 
    // i.e. max el is the 1000th closest pair.
    // 
    // Each el of the form: (dist, (p1, p2))
    //
    std::priority_queue<std::pair<float, std::pair<Pos, Pos>>> pq;
    int n = positions.size();
    for (int i = 0; i < n; i++) {
        for (int j = i+1; j < n; j++) {
            Pos p1 = positions[i];
            Pos p2 = positions[j];
            float dist = eucDistance(p1, p2);

            if (pq.size() < LIMIT) {
                pq.push({dist, {p1, p2}});
            } else {
                auto maxDist = pq.top().first;
                if (dist < maxDist) {
                    pq.pop();
                    pq.push({dist, {p1, p2}});
                }
            }
        }
    }
    assert(pq.size() == LIMIT);

    //
    // Maintain graph of connected sets, 'graph', of form: setId -> {posX, posY, ...}.
    // Initially will contain only 1-el sets.
    // 
    // Also create mapping of form: posId -> setId, for fast lookup of the set a position is in.
    //
    std::unordered_map<int, std::shared_ptr<PosSet>> graph;     // setId -> {posX, posY, ...}
    std::unordered_map<int, int> posSetMap;                     // posId -> setId 
    std::vector<PosPair> pairs;                                 // stores our 1000 closest pairs
    while (!pq.empty()) {
        auto p = pq.top();
        pq.pop();
        auto pair = p.second;
        auto pos1 = pair.first;
        auto pos2 = pair.second;

        pairs.push_back({pos1, pos2});

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

    assert(graph.size() == posSetMap.size());
    std::cout << "starting size of graph <==> num distinct positions used == " << posSetMap.size() << "\n";

    //
    // Iteratively connect pair by pair, slowly merging graph.
    //
    for (auto &p : pairs) {
        // find the sets pos1 and pos2 are in
        Pos &pos1 = p.first;
        Pos &pos2 = p.second;
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
    }

    //
    // At this point, we've connected the graph of 1000 pairs. Now, find top 3 set sizes.
    //
    std::priority_queue<int64_t, std::vector<int64_t>, std::greater<int64_t>> sizes;
    for (auto &kv : graph) {
        auto posSetSize = kv.second->size();
        if (sizes.size() < 3) {
            sizes.push(posSetSize);
        } else {
            auto minTopSize = sizes.top();
            if (posSetSize > minTopSize) {
                sizes.pop();
                sizes.push(posSetSize);
            }
        }
    }

    //
    // Multiply top 3 set sizes together to get our answer.
    //
    int64_t res = 1;
    while (!sizes.empty()) {
        auto s = sizes.top();
        std::cout << s << "\n";
        sizes.pop();
        res *= s;
    }

    return res;
}

int main() {
    int64_t res = solve();
    std::cout << res << "\n";
    return 0;
}