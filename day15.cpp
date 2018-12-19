#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

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

string ObjectChars = ".#EG";
void ReadFile(vector<vector<Object>>& grid, vector<Unit>& units) {
    ifstream inFile("day15Input.txt");
    if(inFile.is_open()) {
        string line;
        int y = 0; int x = 0;
        while(getline(inFile, line)) {
            vector<Object> curLine;
            x = 0;
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
                x++;
            }
            y++;
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
bool MoveUnit(vector<Unit>& units, uint curUnit, uint& target) {
    bool inRange = false;
    // Identify all possible targets
    // adjacent squares of the target are in range
    //  - end turn if no square adjacent and not in range

    return inRange;
}

void Attack(vector<Unit>& units, uint curUnit, uint target) {

}

int main(int argc, char** argv) {
    
    vector<vector<Object>> grid;          // the game grid.
    vector<Unit> units;
    ReadFile(grid, units);
    cout << "Num Units: " << units.size() << endl;

    uint round = 0;
    bool targetsRem = true;
    while (targetsRem) {
        targetsRem = false;
        sort (units.begin(), units.end(), [](const Unit& A, Unit& B) {
            if (A.ypos == B.ypos) {
                return A.xpos < B.xpos;
            } else {
                return A.ypos < B.ypos;
            }
        });

        for (uint i = 0; i < units.size(); i++) {
            for (uint j = 0; j < units.size(); j++) {
                if ((i != j) && (units[i].type != units[j].type)) {
                    targetsRem = true;
                    // move
                    uint target = 0;
                    if (MoveUnit(units, i, target)) {
                        Attack(units, i, target);
                    }
                    // attack
                }
            }
        }

        round++;
    }
}