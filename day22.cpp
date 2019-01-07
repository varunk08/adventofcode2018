#include <string>
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
    rocky = 0,
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
            return ((uint64_t)t.coord.x)<< 32 | (uint64_t)t.coord.y | (t.gear ^ 256);
        }
    };
    template<> struct hash<Coord> {
        size_t operator()(const Coord& c) const noexcept {
            return ((uint64_t)c.x)<<32 | (uint64_t)c.y;
        }
    };
};

const int switchCost = 7;
const int walkCost   = 1;
    int cave[1001][1001];
    int level[1001][1001];
int main(int argc, char** argv) {
    int depth   = stoi(argv[1]);
    int targetX = stoi(argv[2]);
    int targetY = stoi(argv[3]);
    //int depth   = 8103;
    //int targetX = 9;   
    //int targetY = 758; 
    
    //int depth   = 510; 
    //int targetX = 10;  
    //int targetY = 10;  


    //vector<vector<Region>> cave(depth, vector<Region>(depth, {0}));


    for(int i = 0; i <= 1000; i++) {
        for(int j = 0; j <= 1000; j++) {
            int geoIndex;
            if(i == 0) {
                geoIndex = j * 48271;
            } else if (j == 0) {
                geoIndex = i * 16807;
            } else if (i == 0 && j == 0) {
                geoIndex = 0;
            } else if (i == targetX && j == targetY) {
                geoIndex = 0;
            } else {
                geoIndex = level[i-1][j] * level[i][j-1];
            }
            int eroLevel = (geoIndex + depth) % 20183;
            level[i][j] = eroLevel;
            int type = eroLevel % 3;
            cave[i][j] = type;
        }
    }

    
    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 10; x++) {        
            switch(cave[x][y]) {
                case rocky: cout << "."; break;
                case wet: cout << "="; break;
                case narrow: cout << "|"; break;
            }            
        }
        cout << endl;
    }
    
    //cout << "setting target: " << cave[targetX][targetY].erosion % 3  << endl;

    int totalRisk = 0;
    for (int x = 0; x <= targetX; x++) {
        for (int y = 0; y <= targetY; y++) {
            totalRisk += cave[x][y];
        }
    }

    cout << "risk: " << totalRisk << endl;
    Coord target = {targetX, targetY };

    vector<vector<GearStatus>> terrainGearMap(RegionType::count, vector<GearStatus>(2, neither));
    terrainGearMap[rocky]  = { torch, climbing };
    terrainGearMap[wet]    = { neither, climbing };
    terrainGearMap[narrow] = { torch, neither };

    Coord bounds = {targetX + 300, targetY + 300};

        unordered_map<Node, int> minCostToNode;
    auto cmp = [](const Vertex& A, const Vertex& B) { return A.dist > B.dist; };
    priority_queue<Vertex, vector<Vertex>, decltype(cmp) > nodes(cmp);
    Node targetNode = { target, torch };
    Vertex source  =  { { Coord{0, 0}, torch} , 0 }; //Vertex source2  =  { { Coord{0, 0}, climbing} , switchCost };
    minCostToNode.emplace(source.node, 0);           //minCostToNode.emplace(source2.node, source2.dist);
    nodes.push(source);                              //nodes.push(source2);

    while (nodes.empty() == false) {
        Vertex minVert = nodes.top();
        //cout << minVert.dist << " ";
        Node& minNode = minVert.node;
        nodes.pop();
        //if (minVert.dist > minCostToNode.find(minNode)->second) continue;

        if (minNode == targetNode) {
            cout << "Gear: " << minNode.gear << " Dist: " << minVert.dist << endl;
           break;
        }

        const Coord& src = minNode.coord;
        for(Coord adj : {
            Coord {src.x - 1, src.y}, Coord {src.x, src.y + 1}, Coord {src.x + 1, src.y}, Coord {src.x, src.y - 1}
        }) {
            if (adj.x >= 0 &&
                adj.x <= bounds.x &&
                adj.y >= 0 &&
                adj.y <= bounds.y) {
                    int adjRegion = cave[adj.x][adj.y];
                    const vector<GearStatus>& adjGears = terrainGearMap[adjRegion];

                    for (auto gear : adjGears) {
                        // find cost to the node
                        if (gear == minNode.gear) {
                            //nextVert.dist += switchCost;
                            Vertex nextVert  = {{adj, gear}, walkCost + minVert.dist};    
                            auto iter = minCostToNode.find(nextVert.node);
                            if (iter != minCostToNode.end()) {
                                if (nextVert.dist < iter->second) {
                                    iter->second = nextVert.dist;
                                }
                            } else {
                                minCostToNode.emplace(nextVert.node, nextVert.dist);
                                nodes.push(nextVert);
                            }
                            break;          
                        }
                    }
            }
        }


        //cout << "Visiting: " << minNode.coord.x << " , " << minNode.coord.y << "(" << minNode.gear << ") " << minVert.dist << endl;
        
        int region = cave[src.x][src.y];
        const vector<GearStatus>& gears = terrainGearMap[region];
        for (auto gear : gears) {
            if (minNode.gear != gear) {
                Vertex nextVert  = {{src, gear}, switchCost + minVert.dist};
                auto iter = minCostToNode.find(nextVert.node);
                if (iter != minCostToNode.end()) {
                    if (nextVert.dist < iter->second) {
                        iter->second = nextVert.dist;
                    }
                } else {
                    minCostToNode.emplace(nextVert.node, nextVert.dist);
                    nodes.push(nextVert);
                }
                break;
            }
        }


        

    }

    auto iter = minCostToNode.find(targetNode);
    int costToTargetTorch = iter->second;
    cout << "Target 1 (torch) cost: " << iter->second << endl;

    iter  = minCostToNode.find({Coord{target.x, target.y}, climbing});   
    int costToTargetClimb = iter->second;
    cout << "Target 2 (climbing) cost: " << costToTargetClimb << endl; 

    return 0;
}