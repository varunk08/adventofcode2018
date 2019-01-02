#include <iostream>
#include <vector>
#include <algorithm>
#include <deque>
#include <unordered_set>
#include <unordered_map>
#include <limits>
#include <queue>

using namespace std;
enum RegionType {
    rocky,
    wet,
    narrow,
    count
};
enum GearStatus {
    neither = 0,  // wet, narrow
    torch,    // rocky, narrow
    climbing  // rocky, wet
};

struct Region {
    Region(int x) :  erosion(x) {}
    int erosion;
};
struct Coord {
    int x; int y;
    Coord (int _x, int _y): x(_x), y (_y) {};
    bool operator==(const Coord& other) const {
        return x == other.x && y == other.y;
    };
};

struct Node {
    Coord      coord;
    GearStatus gear;
    bool operator==(const Node& other) const {
        return coord.x == other.coord.x &&
               coord.y == other.coord.y &&               
               gear    == other.gear;
    }
};
struct Vertex {
    Node node;
    int dist;
};

namespace std {
    // Custom hash for Node
    template<> struct hash<Node> {
        size_t operator()(const Node& t) const noexcept {
            return ((uint64_t)t.coord.x)<< 32 | (uint64_t)t.coord.y | (t.gear ^ 512);
        }
    };
    template<> struct hash<Coord> {
        size_t operator()(const Coord& c) const noexcept {
            return ((uint64_t)c.x)<<32 | (uint64_t)c.y;
        }
    };
};

const int switchCost = 7;
const int walkCost = 1;

// Djkstra's
int CalcMinDist(vector<vector<Region>>&     cave,
                vector<vector<GearStatus>>& terrainGearMap,
                const Coord&                target,
                const Coord&                bounds) {
    unordered_map<Node, int> minCostToNode;
    auto cmp = [](const Vertex& A, const Vertex& B) { return A.dist > B.dist; };
    priority_queue<Vertex, vector<Vertex>, decltype(cmp) > nodes(cmp);
    unordered_set<Node>      visited;

    Node targetNode = { target, torch };
    Vertex source  =  { { Coord{0, 0}, torch} , 0 };
    minCostToNode.emplace(source.node, 0);
    nodes.push(source);

    //Node source1 = { Coord{0, 0}, climbing };    
    //minCostToNode.emplace(source1, switchCost);    
    //nodes.push(source1);    

    while (nodes.empty() == false) {
        Vertex minVert = nodes.top();
        //cout << minVert.dist << " ";
        Node& minNode = minVert.node;
        nodes.pop();

        if (minNode.coord == bounds) {
            break;
        }

        //cout << "Visiting: " << minNode.coord.x << " , " << minNode.coord.y << "(" << minNode.gear << ") " << minVert.dist << endl;
        visited.emplace(minNode);
        const Coord& src = minNode.coord;

        for(Coord adj : {
            Coord {src.x + 1, src.y},Coord {src.x - 1, src.y},Coord {src.x, src.y - 1},Coord {src.x, src.y + 1}
        }) {

            if (adj.x >= 0 &&
                adj.x <= bounds.x &&
                adj.y >= 0 &&
                adj.y <= bounds.y) {
                    int adjRegion = cave[adj.x][adj.y].erosion % 3;
                    const vector<GearStatus>& adjGears = terrainGearMap[adjRegion];

                    for (int i = 0; i < adjGears.size(); ++i) {
                        Vertex nextVert  = {{adj, adjGears[i]}, walkCost + minVert.dist};

                        if (visited.find(nextVert.node) == visited.end()) {
                            // find cost to the node
                            if (adjGears[i] != minNode.gear) {
                                nextVert.dist += switchCost;
                            }

                            auto iter = minCostToNode.find(nextVert.node);
                            if (iter != minCostToNode.end()) {
                                if (iter->second > nextVert.dist) {
                                    iter->second = nextVert.dist;
                                }
                            } else {
                                minCostToNode.emplace(nextVert.node, nextVert.dist);
                                nodes.push(nextVert);
                            }
                        }
                    }
            }
        }

    }

    auto iter = minCostToNode.find(targetNode);
    int costToTargetTorch = iter->second;
    cout << "Target 1 (torch) cost: " << iter->second << endl;

    iter  = minCostToNode.find({Coord{target.x, target.y}, climbing});   
    int costToTargetClimb = iter->second;
    costToTargetClimb += switchCost;
    cout << "Target 2 (climbing) cost: " << costToTargetClimb << endl;    

    return min(costToTargetClimb, costToTargetTorch);
}

int main(int argc, char** argv) {
    //int depth   = 8103; // 510; 
    //int targetX = 9;    // 10;  
    //int targetY = 758;  // 10;  
    
    int depth   = 510; 
    int targetX = 10;  
    int targetY = 10;  

    vector<vector<Region>> cave(depth, vector<Region>(depth, {0}));
    
    for (int x = 0; x < depth; x++) {
        for (int y = 0; y < depth; y++) {
            int geologicIdx = 0;
            if (x == 0) {
                geologicIdx = y * 48271;
            } else if (y == 0) {
                geologicIdx = x * 16807;                
            } else {
                geologicIdx = cave[x-1][y].erosion * cave[x][y-1].erosion;
            }

            cave[x][y].erosion = (geologicIdx + depth) % 20183;
        }
    }

    cave[0][0].erosion             = (0 + depth) % 20183;
    cave[targetX][targetY].erosion = (0 + depth) % 20183;
    
    //cout << "setting target: " << cave[targetX][targetY].erosion % 3  << endl;

    int totalRisk = 0;
    for (int x = 0; x <= targetX; x++) {
        for (int y = 0; y <= targetY; y++) {
            totalRisk += cave[x][y].erosion % 3;
        }
    }
    cout << "risk: " << totalRisk << endl;

    vector<vector<GearStatus>> terrainGearMap(RegionType::count, vector<GearStatus>(2, neither));
    terrainGearMap[rocky]  = { torch, climbing };
    terrainGearMap[wet]    = { neither, climbing };
    terrainGearMap[narrow] = { torch, neither };

    int minTime = CalcMinDist(cave, terrainGearMap, {targetX, targetY}, {targetX + 20, targetY + 20});
    cout << "[Main] Min time to target: " << minTime << endl;

    return 0;
}