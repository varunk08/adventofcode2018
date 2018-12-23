#include <iostream>
#include <vector>
#include <algorithm>
#include <deque>
#include <unordered_set>
#include <unordered_map>
#include <limits>

using namespace std;
enum RegionType {
    rocky,
    wet,
    narrow
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
enum GearStatus {
    neither,  // wet, narrow
    torch,    // rocky, narrow
    climbing  // rocky, wet
};
struct TraversalInfo {
    Coord      coord;
    Coord      src;
    int        costToNode;
    GearStatus prevGear;
    bool operator==(const TraversalInfo& other) const {
        return coord.x == other.coord.x &&
               coord.y == other.coord.y &&
               costToNode == other.costToNode &&
               prevGear   == other.prevGear;
    }
};
namespace std {
    // Custom hash for Traversal info. no idea what's going on here!
    template<> struct hash<TraversalInfo> {
        size_t operator()(const TraversalInfo& t) const noexcept {
            return ((uint64_t)t.coord.x)<<32 | (uint64_t)t.coord.y |
                    (t.costToNode ^ 1024) | (t.prevGear ^ 512);
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

bool IsInvalidGear(const RegionType region, const GearStatus& gear) {
    bool invalid = false;
    switch(region) {
        case rocky:  invalid = (gear == neither); break;
        case wet:    invalid = (gear == torch); break;
        case narrow: invalid = (gear == climbing); break;
    }
    return invalid;
}

void ShortestPathHelper(vector<vector<Region>>& cave,
                       deque<TraversalInfo>& nodeQueue, GearStatus curGear,
                       const Coord& start, const Coord& target) {
    unordered_set<TraversalInfo> visited;
    unordered_map<Coord, int> minCostToNode;

    nodeQueue.push_back({start, {-1,-1},  0, torch});  // root node, the source
    minCostToNode.emplace(start, numeric_limits<int>::max());

    while(nodeQueue.empty() == false) {
        TraversalInfo cur = nodeQueue.front();      // get a node
        nodeQueue.pop_front();
        //cout << nodeQueue.size() << endl;
        auto iter = visited.find(cur);
        if (iter == visited.end()) {
            visited.emplace(cur);
            RegionType curRegion = static_cast<RegionType>(cave[cur.coord.x][cur.coord.y].erosion % 3);
            cout << "visiting: " << cur.coord.x << ", "  << cur.coord.y << " " << cur.costToNode << endl;
            if (cur.coord.x == target.x && cur.coord.y == target.y) {
                cout << "target reached" << endl;
                // for the target, the actual cost for switching needs to be included.
                int cost = cur.costToNode;
                if (cur.prevGear != torch) {
                        cost += switchCost;
                }
 
                auto it = minCostToNode.find(cur.coord);
                if (it != minCostToNode.end()) {
                    it->second = min(it->second, cost);
                } else {
                    minCostToNode.emplace(cur.coord, cost);
                }
                cout << "Cost to target: " << cost << endl;

            } else {
                // add the actual cost to node prior to switching decisions for other nodes.
                int costToNodeIncludingSwith = cur.costToNode;
                bool isInvalidGear = IsInvalidGear(curRegion, cur.prevGear);
                if (isInvalidGear) {
                    costToNodeIncludingSwith += switchCost;
                }

                auto it = minCostToNode.find(cur.coord);
                bool chooseThisNode = false;
                if (it != minCostToNode.end()) {
                    if (costToNodeIncludingSwith < it->second) {
                        it->second = costToNodeIncludingSwith;
                        //cout << "replacing minCostToNode: " << cur.coord.x << ", " << cur.coord.y << " "<< costToNodeIncludingSwith <<endl;
                        chooseThisNode = true;
                    }                    
                } else {
                    //cout << "adding to minCostToNode: " << cur.coord.x << ", " << cur.coord.y << " "<< costToNodeIncludingSwith <<endl;
                    minCostToNode.emplace(cur.coord, costToNodeIncludingSwith);
                    chooseThisNode = true;
                }                

                if (chooseThisNode) {
                // continue to add child nodes when the cur node is not the target
                for (const Coord& next : { Coord {cur.coord.x - 1, cur.coord.y},
                                           Coord {cur.coord.x + 1, cur.coord.y},
                                           Coord {cur.coord.x, cur.coord.y - 1},
                                           Coord {cur.coord.x, cur.coord.y + 1},}) {
                    if (next.x >= 0 && next.x < cave.size() && next.y >= 0 && next.y < cave[0].size() &&
                        ((cur.src == next) == false)) {
                        // for each node, create two nodes (one which switches, the other which doesn't)
                        TraversalInfo newNodeWithGear1 = {next, cur.coord, cur.costToNode + walkCost, cur.prevGear};
                        TraversalInfo newNodeWithGear2 = {next, cur.coord, cur.costToNode + walkCost, cur.prevGear};
                            
                        if(isInvalidGear) {
                            switch (curRegion) {
                            case rocky:
                                newNodeWithGear1.prevGear = torch;
                                newNodeWithGear2.prevGear = climbing;
                                break;
                            case wet:
                                newNodeWithGear1.prevGear = neither;
                                newNodeWithGear2.prevGear = climbing;
                                break;
                            case narrow:
                                newNodeWithGear1.prevGear = torch;
                                newNodeWithGear2.prevGear = neither;
                                break;
                            }
                            newNodeWithGear1.costToNode += switchCost;
                            newNodeWithGear1.costToNode += switchCost;
                        } else {
                            // one of the new nodes will have the same gear as the previous, with cost
                            // switch not added.
                            switch (curRegion) {
                            case rocky:
                                newNodeWithGear1.prevGear = (cur.prevGear == torch) ? climbing : torch;
                                break;
                            case wet:
                                newNodeWithGear1.prevGear = (cur.prevGear == climbing) ? neither : climbing;
                                break;
                            case narrow:
                                newNodeWithGear1.prevGear = (cur.prevGear == torch) ? neither : torch;
                                break;
                            }
                            // no switch cost for new node 2.
                            newNodeWithGear2.prevGear = cur.prevGear;
                            // add switch cost for new node 1
                            newNodeWithGear1.costToNode += switchCost;
                        }

                        nodeQueue.push_back(newNodeWithGear1);
                        nodeQueue.push_back(newNodeWithGear2);
                    } // valid coord
                } // for each adj coord
            }
            } // non target node
        }        
    }
}

void FindShortestPath(vector<vector<Region>>& cave, const Coord& start, const Coord& target) {
    deque<TraversalInfo> nodeQueue;
    GearStatus initStatus = torch;
    ShortestPathHelper(cave, nodeQueue, initStatus, start, target);
}

int main(int argc, char** argv) {
    int depth = 510;//8103;
    int targetX = 10; //9;
    int targetY = 10;//758;
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

    cave[0][0].erosion = (0 + depth) % 20183;
    cave[targetX][targetY].erosion = (0 + depth) % 20183;

    int totalRisk = 0;
    for (int x = 0; x <= targetX; x++) {
        for (int y = 0; y <= targetY; y++) {
            totalRisk += cave[x][y].erosion % 3;
        }
    }
    cout << "risk: " << totalRisk << endl;

    FindShortestPath(cave, {0, 0}, {targetX, targetY});
    //cout << "Total time: " << time << endl;

    return 0;
}