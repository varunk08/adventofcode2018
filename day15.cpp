#include <limits>
#include <queue>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>

using namespace std;
using uint = unsigned int;
enum Object {
    Empty,
    Wall,
    Elf,
    Goblin
};
struct Unit {
    int xpos; int ypos; Object type; int hp;
};
struct  Coord {
    int x; int y;
    bool operator==(const Coord& other) const noexcept {
        return x == other.x && y == other.y;
    }
    bool operator!=(const Coord& other) const noexcept {
        return x != other.x && y != other.y;
    }
    friend ostream& operator<<(ostream& os, const Coord& c)
    {
        os << "(" << c.x << ", " << c.y << ")";
        return os;
    }
};

namespace std {
    template<> struct hash<Coord> {
        size_t operator()(const Coord& c) const noexcept {
            return ((uint64_t)c.x)<<32 | (uint64_t)c.y;
        }
    };
};

string ObjectChars = ".#EG";
void ReadFile(vector<vector<Object>>& grid, vector<Unit>& units) {
    ifstream inFile("day15Input.txt");
    if(inFile.is_open()) {
        string line;
        int y = 0; int x = 0;
        while(getline(inFile, line)) {
            vector<Object> curLine;
            y = 0;
            for (auto ch : line) {
                switch(ch) {
                    case '#': curLine.push_back(Wall); break;
                    case '.': curLine.push_back(Empty); break;
                    case 'E': {
                        curLine.push_back(Elf);
                        Unit elf;
                        elf.xpos = x;
                        elf.ypos = y;
                        elf.type = Object::Elf;
                        elf.hp = 200;
                        units.push_back(elf);
                        break;
                    }
                    case 'G': {
                        curLine.push_back(Goblin);
                        Unit gob;
                        gob.xpos = x;
                        gob.ypos = y;
                        gob.type = Object::Goblin;
                        gob.hp = 200;
                        units.push_back(gob);
                     break;
                    }
                }
                y++;
            }
            x++;
            grid.push_back(curLine);
        }
    }

    for (auto vec : grid) {
        for(auto obj : vec) {
            cout << ObjectChars[obj];
        }
        cout << endl;
    }
}

// Returns true if unit in range of enemy
bool MoveUnit(vector<vector<Object>>& grid, vector<Unit>& units, uint curUnit) {
    bool inRange = false;
    Unit& cur = units[curUnit];
    Coord root = {cur.xpos, cur.ypos};

    // Identify all possible targets
    vector<Coord> allAdjPts;
    for (uint i = 0; i < units.size(); i++) {
        if (i != curUnit && units[i].type != cur.type) {
            // adjacent squares of the target are in range
            for (const Coord& adjPt : {
                Coord {units[i].xpos - 1, units[i].ypos},
                Coord {units[i].xpos + 1, units[i].ypos},
                Coord {units[i].xpos, units[i].ypos - 1},
                Coord {units[i].xpos, units[i].ypos + 1}
            }) {
                if (adjPt.x >= 0 && adjPt.x < grid.size() && adjPt.y >= 0 && adjPt.y < grid[adjPt.x].size() &&
                    (grid[adjPt.x][adjPt.y] == Empty)) {  // its a valid coord
                    allAdjPts.push_back(adjPt);
                }
            }
        }
    }

    for (auto targetPt : allAdjPts) {
        cout << "[Move] Target: " << targetPt << endl;
    }

    // find shortest path to each target point
       
    int minDist        = numeric_limits<int>::max();
    Coord nextMove     = {0, 0};
    Coord prevTargetPt = {units[curUnit].xpos, units[curUnit].ypos};

    // maps node and visited from
    unordered_map<Coord, Coord> path;
    unordered_set<Coord>        visited;
    for (auto targetPt : allAdjPts) {
        cout << "[Move] trying to find " << targetPt << endl;  
        path.clear();
        visited.clear();
        queue<Coord> nextNodes; 

        nextNodes.emplace(Coord{cur.xpos, cur.ypos});
        // the root is visited from none
        path.emplace(root, Coord{-1, -1});

        while(nextNodes.empty() == false) {
            Coord curLoc = nextNodes.front();
            nextNodes.pop();

            auto it = visited.find(curLoc);
            if (it == visited.end()) {
                visited.emplace(curLoc);

                if (curLoc == targetPt) {
                    cout << "[Move] target found!" << endl;
                    break;
                }

                for (Coord next : {
                    Coord {curLoc.x - 1, curLoc.y},
                    Coord {curLoc.x + 1, curLoc.y},
                    Coord {curLoc.x, curLoc.y - 1},
                    Coord {curLoc.x, curLoc.y + 1}
                }) {
                    if (next.x >= 0 && next.x < grid.size() && next.y >= 0 && next.y < grid[next.x].size() &&
                    (grid[next.x][next.y] == Empty)) {  // its a valid coord
                        path.emplace(next, curLoc);
                        nextNodes.emplace(Coord {next.x, next.y});
                    }
                } 
            }
        }
    
        if (path.size() <= 1) {
            continue;
        }

        // back track to get distance to root
        auto pit = path.find(targetPt);
        if (pit != path.end()) {
            Coord from = pit->second;
            int dist = 1;
            while (from != root) {
                pit = path.find(from);
                if (pit != path.end()) {
                    from = pit->second;
                    dist++;
                } else {
                    cout << "[Move] path map can't find! " << targetPt << " " << root << " " << from << endl;
                }
            }

            if (dist < minDist) {
                minDist = dist;
                nextMove = pit->first;
                prevTargetPt = targetPt;
            } else if (dist == minDist) {
                // break tie based on reading order of target point
                if (prevTargetPt.y == targetPt.y) {
                    if (prevTargetPt.x > targetPt.x) {
                        prevTargetPt = targetPt;
                        nextMove = pit->first;
                    }
                } else if (prevTargetPt.y > targetPt.y) {
                    prevTargetPt = targetPt;
                    nextMove = pit->first;
                }
            }
            cout << "[Move] currently chosen target: " << prevTargetPt << endl;
        } else {
            cout << "[Move] target not found!" << endl;
        }
    }

    /*
    // Debug print stuff here:
    cout << "[Move] Dist: " << minDist << endl;
    cout << "[Move] Next move: " << nextMove << endl;
    cout << "[Move] Target pt: " << prevTargetPt << endl;

    for (int i = 0; i < grid.size(); i++) {
        for(int j = 0; j < grid[i].size(); j++) {
            char ch = grid[i][j];
            for (auto pt : allAdjPts) {
                if (i == pt.x && j == pt.y) {
                    ch = 'x';
                    break;
                }
            }
            cout << ch;
        }
        cout << endl;
    }   
    */

    return inRange;
}

void Attack(vector<Unit>& units, uint curUnit, uint target) {

}

int main(int argc, char** argv) {
    // the game grid
    vector<vector<Object>> grid;
    // all units
    vector<Unit> units;
    // read the file and fill in the data structures
    ReadFile(grid, units);
    cout << "Num Units: " << units.size() << endl;

    uint round = 0;
    bool targetsRem = true;

    while (targetsRem) {
        targetsRem = false;
        // sort the units for correct move order
        sort (units.begin(), units.end(), [](const Unit& A, Unit& B) {
            if (A.ypos == B.ypos) {
                return A.xpos < B.xpos;
            } else {
                return A.ypos < B.ypos;
            }
        });

        // Perform move or attack for each unit
        for (uint i = 0; i < units.size(); i++) {
            targetsRem = MoveUnit(grid, units, i);
            // Test:
            targetsRem = false; break;
        }

        round++;
    }
}