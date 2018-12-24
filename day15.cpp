#include <limits>
#include <queue>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>

#define ENABLE_DBG_PRINT 0

using namespace std;
using uint = unsigned int;

const uint UnitHp       = 200;
const uint GobAttackPwr = 3;
const uint ElfAttackPwr = 40;

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
        return (*this == other) == false;
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
    vector<vector<Object>> temp;
    if(inFile.is_open()) {
        string line;
        while(getline(inFile, line)) {
            vector<Object> curLine;
            for (auto ch : line) {
                switch(ch) {
                    case '#': curLine.push_back(Wall); break;
                    case '.': curLine.push_back(Empty); break;
                    case 'E': curLine.push_back(Elf); break;
                    case 'G': curLine.push_back(Goblin); break;                    
                }
            }
            temp.push_back(curLine);
        }
    }

    cout <<"[Readfile] " << temp.size() * temp[0].size() <<endl;
    grid.resize(temp.size());
    for (int i = 0; i < grid.size(); i++) {
        grid[i].resize(temp[0].size());
    }

    for (int y = 0; y < temp.size(); y++) {
        for (int x = 0; x < temp[y].size(); x++) {
            grid[x][y] = temp[y][x];
            switch(grid[x][y]) {
                case Wall : break;
                case Empty : break;
                case Elf : {
                    Unit elf;
                    elf.xpos = x;
                    elf.ypos = y;
                    elf.type = Object::Elf;
                    elf.hp   = UnitHp;
                    units.push_back(elf);
                    break;
                    }
                case Goblin : {
                    Unit gob;
                    gob.xpos = x;
                    gob.ypos = y;
                    gob.type = Object::Goblin;
                    gob.hp   = UnitHp;
                    units.push_back(gob);
                    break;
                    }
            }
        }
    }

    for (int y = 0; y < grid.size(); y++) {
        for (int x = 0; x < grid[y].size(); x++) {
            cout << ObjectChars[grid[x][y]];
        }
        cout << endl;
    }
    /*for (auto vec : grid) {
        for(auto obj : vec) {
            cout << ObjectChars[obj];
        }
        cout << endl;
    }*/
}

struct MoveStatus {
    bool alreadyInRange;
    bool foundSomebody;   // the unit was stuck or moved this turn
};

// Returns true if unit in range of enemy
MoveStatus MoveUnit(vector<vector<Object>>& grid, vector<Unit>& units, uint curUnit) {
    MoveStatus move    = {false, false};
    Unit& cur          = units[curUnit];
    Coord root         = {cur.xpos, cur.ypos};
    int minDist        = numeric_limits<int>::max();
    Coord nextMove     = {-1, -1};
    Coord prevTargetPt = root;

    // Identify all possible targets
    vector<Coord> allAdjPts;
    for (uint i = 0; (i < units.size()) && (move.alreadyInRange == false); i++) {
        if (i != curUnit && units[i].type != cur.type) {
            move.foundSomebody = true;
            // adjacent squares of the target are in range
            for (const Coord& adjPt : {                
                Coord {units[i].xpos, units[i].ypos - 1},
                Coord {units[i].xpos + 1, units[i].ypos},
                Coord {units[i].xpos - 1, units[i].ypos},                             
                Coord {units[i].xpos, units[i].ypos + 1}
            }) {
                    if (adjPt.x >= 0 && adjPt.x < grid.size() && adjPt.y >= 0 && adjPt.y < grid[adjPt.x].size()) {
                        if (grid[adjPt.x][adjPt.y] == Empty) {  // its a valid coord
                            allAdjPts.push_back(adjPt);                    
                        } else if (root == adjPt) {
                                // Early exit case for already in range condition
                                //cout << "[Move] unit already in range" << endl;
                                nextMove            = root;
                                prevTargetPt        = root;
                                minDist             = 0;
                                move.alreadyInRange = true;
                                break;
                    }
                }
            }
        }
    }
    

    // the unit is already in range, or it found nobody!
    if (move.alreadyInRange || (move.foundSomebody == false)) {
        return move;
    }

    for (auto targetPt : allAdjPts) { 
        for (Coord adjRoot : {
            Coord {root.x, root.y - 1},                    
            Coord {root.x + 1, root.y},  
            Coord {root.x - 1, root.y},                                      
            Coord {root.x, root.y + 1} }) {
            
            if (adjRoot.x >= 0 && adjRoot.x < grid.size() && adjRoot.y >= 0 && adjRoot.y < grid[adjRoot.x].size() &&
                (grid[adjRoot.x][adjRoot.y] == Empty)) {  // its a valid coord

                unordered_map<Coord, Coord> path;
                unordered_set<Coord>        visited;
                queue<Coord>                nextNodes;


                nextNodes.emplace(adjRoot);                
                // the root is visited from the actual root
                path.emplace(adjRoot, root);

                while(nextNodes.empty() == false) {
                    Coord curLoc = nextNodes.front();
                    nextNodes.pop();

                    auto it = visited.find(curLoc);
                    if (it == visited.end()) {
                        visited.emplace(curLoc);
    
                            if (curLoc == targetPt) {
                                break; // continue here as there could be multiple shortest paths
                            }
    
                        for (Coord next : {                    
                            Coord {curLoc.x, curLoc.y - 1},                    
                            Coord {curLoc.x + 1, curLoc.y},  
                            Coord {curLoc.x - 1, curLoc.y},                                      
                            Coord {curLoc.x, curLoc.y + 1}
                        }) {
                            if (next.x >= 0 && next.x < grid.size() && next.y >= 0 && next.y < grid[next.x].size() &&
                            (grid[next.x][next.y] == Empty)) {  // its a valid coord
                                path.emplace(next, curLoc);
                                nextNodes.emplace(Coord {next.x, next.y});
                            }
                        } 
                    }
                } // find the shortest path
        
                // back track to get distance to root
                auto pit = path.find(targetPt);
                if (pit != path.end()) {
                    int dist = 0;
                    //cout << "[Move] target: " << targetPt << " from: " << from <<" root: " << adjRoot << " dist: " << dist << endl;
                    while (pit->second != root) {
                        pit = path.find(pit->second);
                        if (pit != path.end()) {
                            dist++;
                        } else {
                            cout << "[Move] path map can't find! " << targetPt << " " << root << " " << pit->second << endl;
                        }
                    } // back track through path map
                    
                    if (dist < minDist) {
                        minDist      = dist;
                        nextMove     = adjRoot;
                        prevTargetPt = targetPt;
                    } else if (dist == minDist) {
                        // break tie based on reading order of target point
                        if (prevTargetPt.y == targetPt.y) {
                            if (prevTargetPt.x == targetPt.x) { // same targetPt with different paths of same distance
                                if (adjRoot.y < nextMove.y) {
                                    nextMove = adjRoot;

                                } else if ((adjRoot.y == nextMove.y) && (adjRoot.x < nextMove.x)) {
                                    nextMove = adjRoot;
                                }

                            } else if (prevTargetPt.x > targetPt.x) { // different target in the same x coord
                                prevTargetPt = targetPt;
                                nextMove = adjRoot;
                            }

                        } else if (prevTargetPt.y > targetPt.y) {
                            prevTargetPt = targetPt;
                            nextMove = adjRoot;
                        }                        
                    } // if there was a better path

                } // if there was a valid path

            } // adj point is valid

        } // for each adj point to the root        
    } // for each target

#if ENABLE_DBG_PRINT
    // Debug print stuff here:
    cout << "[Move] Dist: " << minDist << endl;
    cout << "[Move] Next move: " << nextMove << endl;
    cout << "[Move] Target pt: " << prevTargetPt << endl;
    
    for (int y = 0; y < grid.size(); y++) {
        for(int x = 0; x < grid[y].size(); x++) {
            char ch = grid[x][y];
            for (auto pt : allAdjPts) {
                if (x == pt.x && y == pt.y) {
                    ch = 'x';
                    break;
                }
            }
            cout << ch;
        }
        cout << endl;
    }   
#endif

    if (prevTargetPt != root && nextMove != Coord{ -1, -1}) {
        grid[cur.xpos][cur.ypos]     = Empty;
        units[curUnit].xpos          = nextMove.x;
        units[curUnit].ypos          = nextMove.y;
        grid[nextMove.x][nextMove.y] = cur.type;

        if (nextMove == prevTargetPt) {
            move.alreadyInRange = true; // unit moved into range this turn, so attack
        }
    }

    return move;
}

struct  AttackOutcome
{
    bool someoneDied;
    uint diedIdx;
    bool elfDied;  
};
AttackOutcome Attack(vector<vector<Object>>& grid, vector<Unit>& units, uint curUnit, int elfAttack) {
    AttackOutcome res  = {false,  -1};
    Unit& cur          = units[curUnit];
    Coord root         = {cur.xpos, cur.ypos};
    Object enemy       = (units[curUnit].type == Goblin) ? Elf : Goblin;

    // find all valid adjacent coords
    vector<Coord> validAdjCoords;
    for (Coord adjCoord : {
        Coord {cur.xpos - 1, cur.ypos},
        Coord {cur.xpos + 1, cur.ypos},
        Coord {cur.xpos, cur.ypos - 1},
        Coord {cur.xpos, cur.ypos + 1}}) {
        if (adjCoord.x >= 0 && adjCoord.x < grid.size() && adjCoord.y >= 0 && adjCoord.y < grid[0].size() &&
            (grid[adjCoord.x][adjCoord.y] == enemy) ) {
            validAdjCoords.push_back(adjCoord);
        }
    }

    // find enemies in valid adjacent coords and choose an enemy
    uint chosenIdx = -1;
    for (int i = 0; (i < units.size()) && (validAdjCoords.size() > 0); i++) {
        if (i != curUnit && units[i].type == enemy) {            
            for (auto coord : validAdjCoords) {
                if (units[i].xpos == coord.x && units[i].ypos == coord.y) {
                    if (chosenIdx == -1) {
                        chosenIdx = i;
                    } else {
                        if (units[i].hp == units[chosenIdx].hp) {
                            if (units[i].ypos == units[chosenIdx].ypos) { // choose reading order if the hp is the same
                                chosenIdx = (units[i].xpos < units[chosenIdx].xpos) ? i : chosenIdx;
                            }
                        } else if (units[i].hp < units[chosenIdx].hp) {
                            chosenIdx = i;
                        }
                    }                        
                }
            }
        }
    }            

    // perform the attack
    if (chosenIdx != -1) {
        units[chosenIdx].hp -= (units[chosenIdx].type == Elf) ? GobAttackPwr : elfAttack;

        // erase the enemy unit from the map
            if (units[chosenIdx].hp <= 0) {
                grid[units[chosenIdx].xpos][units[chosenIdx].ypos] = Empty;                
                
                if (units[chosenIdx].type == Elf) {
                    res.elfDied = true;
                }

                int idx = 0;
                for (auto it = units.begin(); it != units.end(); it++) {
                   if (idx++ == chosenIdx) {
                       it = units.erase(it);
                       break;
                   }            
                }

                if (curUnit > idx) {
                    chosenIdx--;
                }

                res.someoneDied = true;
                res.diedIdx     = chosenIdx;
        }
    } else {
        // can't attack this turn
    }

    return res;
}

int main(int argc, char** argv) {
    // the game grid
    vector<vector<Object>> mainGrid;
    // all units
    vector<Unit> mainUnits;
    // read the file and fill in the data structures
    ReadFile(mainGrid, mainUnits);
    cout << "[Main] Num Units: " << mainUnits.size() << endl;

    uint round = 0;
    bool targetsRem = true;
    bool reset = false;
    int elfPwr = ElfAttackPwr;

    vector<vector<Object>> grid(mainGrid);
    vector<Unit> units(mainUnits);


    while (targetsRem) {

        if (reset) {
            round = 0;
            reset = false;
            grid  = mainGrid;
            units = mainUnits;
            elfPwr++;
        }

        // sort the units for correct move order
        sort (units.begin(), units.end(), [](const Unit& A, Unit& B) {
            if (A.ypos == B.ypos) {
                return A.xpos < B.xpos;
            } else {
                return A.ypos < B.ypos;
            }
        });     

        // Perform move or attack for each unit        
        for (uint i = 0; (i < units.size()) && (targetsRem); i++) {
            //cout << "[Main] unit loc: " << units[i].xpos << ", " << units[i].ypos << endl;
            MoveStatus move = MoveUnit(grid, units, i);          
            if (move.alreadyInRange) {
                AttackOutcome result = Attack(grid, units, i, elfPwr);
                if (result.someoneDied) {
                    //cout << "[Main] died: " << i << endl;
                    if (result.diedIdx < i) {
                        i--;                    
                    }

                    if (result.elfDied) {
                        reset = true;
                        break;
                    }

                    targetsRem = false;
                    for (auto unit : units) {
                        if (unit.type != units[i].type) {
                            targetsRem = true;
                        }
                    }
                }
            } else if (move.foundSomebody == false) {
                // didn't move, didn't attack, so enemies vanquished
                targetsRem = false;
                break;
            }
        }

        if (targetsRem) {
            round++;
        }
#if ENABLE_DBG_PRINT
        cout << "[Main] Round: " << round << endl;
        for (auto unit : units) {
            cout << "[Main] unit hp: " << unit.type << " " <<  unit.hp << endl;
        }        
#endif
        cout << endl;
        for (int y = 0; y < grid.size(); y++) {
            for(int x = 0; x < grid[y].size(); x++) {
                    if (grid[x][y] == Wall) {
                        cout << (char) 179;
                    } else if (grid[x][y] == Elf) {
                        cout << (char)(grid[x][y]+1) ;
                    } else if (grid[x][y] == Goblin) {
                        cout << 'x' ;
                    }else {
                       cout << " ";
                    }                
                }
            cout << endl;
        }
        
    }
#if ENABLE_DBG_PRINT
    cout << "[Main] Num rounds: " << round << endl;
    int score = 0;
    for (auto unit : units) {
        cout << "[Main] elf hp: " << unit.hp << endl;
        score += unit.hp;
    }
    cout << "[Main] Total HP: " << score << endl;
    cout << "[Main] Score: " << round * score << endl;
    cout << "[Main] Elf attack: " << elfPwr << endl;
#endif    
    return 0;
}