#include <limits>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <deque>

using namespace std;

struct Range {
    int startX; int endX; int startY; int endY;

    friend ostream& operator<<( ostream& os, const Range& r)  {
        os << "x:(" << r.startX << ", " << r.endX << ") y:(" << r.startY << ", " << r.endY << ")";
        return os;
    }
};
enum Object {
    sand,
    clay,
    water,
    stagnantWater
};

struct Coord {
    int x;
    int y;
};
using Spring = Coord;

void TraceSpring(vector<vector<Object>>& grid, Spring curSpring);

bool TraceLateral(vector<vector<Object>>& grid, Coord cur, int dir) {
    if (grid[cur.x][cur.y] == clay) {
        return true;
    }

    if (cur.y == 0 || cur.y == grid[0].size() - 1 || cur.x == 0 || cur.x == grid.size()) {
        return false;
    }

    if (grid[cur.x][cur.y + 1] == sand) {
        
        if (grid[cur.x - dir][cur.y + 1] != water) {
            TraceSpring(grid, Spring {cur.x, cur.y});
            grid[cur.x][cur.y] = water;

            if ((cur.x < grid.size() - 1) && grid[cur.x + dir][cur.y + 1] == water) {
                return TraceLateral(grid, {cur.x + dir, cur.y}, dir);
            }

            if ((cur.x + dir) >= 0 && (cur.x + dir) < grid.size() && (cur.y + 1) >=0 && (cur.y + 1) < grid[0].size()) {                
                if (grid[cur.x + dir][cur.y + 1] == clay) {
                    return TraceLateral(grid, {cur.x + dir, cur.y}, dir);       
                }
            }
        }

        return false;        
    } else {
        grid[cur.x][cur.y] = water;
    }

    return TraceLateral(grid, {cur.x + dir, cur.y}, dir);
}

void TraceSpring(vector<vector<Object>>& grid, Spring curSpring) {
    deque<Coord> perSpringStack;
    Coord nextDown = {curSpring.x, curSpring.y + 1};
    while ((nextDown.y < grid[0].size()) && (grid[nextDown.x][nextDown.y] != clay)) {
        perSpringStack.push_back(nextDown);        
        grid[nextDown.x][nextDown.y] = water;
        nextDown.y++;
    }

    while (perSpringStack.empty() == false) {        
        Coord cur = perSpringStack.back();
        perSpringStack.pop_back();

        if (cur.y == 0 || cur.y == grid[0].size() - 1 || cur.x == 0 || cur.x == grid.size() - 1) {
            break;
        }

        bool hitWallLeft  = TraceLateral(grid, {cur.x -1, cur.y}, -1);
        bool hitWallRight = TraceLateral(grid, {cur.x +1, cur.y}, +1);

        // one of them didn't hit a wall, so don't process further.
        if (hitWallRight == false || hitWallLeft == false) {
            break;
        }
    }
}

int main(int argc, char** argv) {
    ifstream inFile("day17Input.txt");
    vector<Range> ranges;
    if (inFile.is_open()) {
        string line;
        while(getline(inFile, line)) {            
            Range range;
            if (line[0] == 'x') {
                range.startX = range.endX = stoi(line.substr(line.find('=') + 1, line.find(',')));                
                range.startY = stoi(line.substr(line.find('y')+2, line.find('.')));
                range.endY = stoi(line.substr(line.find('.')+2));
            } else if (line[0] == 'y') {
                range.startY = range.endY = stoi(line.substr(line.find('=') + 1, line.find(',')));
                range.startX = stoi(line.substr(line.find('x')+2, line.find('.')));
                range.endX = stoi(line.substr(line.find('.')+2));
            }
            ranges.push_back(range);
        }
    }

    int minX = numeric_limits<int>::max();
    int maxX = numeric_limits<int>::min();
    int minY = numeric_limits<int>::max();
    int maxY = numeric_limits<int>::min();
    for (auto range : ranges) {
        if (range.startX < minX) { minX = range.startX; }
        if (range.endX > maxX)   { maxX = range.endX;   }
        if (range.startY < minY) { minY = range.startY; }
        if (range.endY > maxY)   { maxY = range.endY;   }
    }

    int width = maxX - minX + 1;
    int height = maxY - minY + 1;
    // +2 for extra space on either extremities
    vector<vector<Object>> grid(width + 2, vector<Object>(height + 1, sand));
    cout << "[Main] x: " << grid.size() << endl;
    cout << "[Main] y: " << grid[0].size() << endl;

/*
    for (int y = 0; y < grid[0].size(); y++) {
        for (int x = 0; x < grid.size(); x++) {
            cout << grid[x][y];
        }
        cout << endl;
    }
*/
    for (auto r : ranges) {
        for (int y = r.startY - minY + 1; y <= r.endY - minY + 1; y++) {
            for(int x = r.startX - minX + 1; x <= r.endX - minX + 1; x++) {
                //cout << r << endl;
                //cout << x << ", " << y << endl;
                grid[x][y] = clay;
            }
        }
    }

    cout << "[Main] starting tracing "<< endl;
    TraceSpring(grid, Spring {500 - minX + 1, 0});    
    
    int numWater = 0;
    for (int y = 0; y < grid[0].size(); y++) {
        for (int x = 0; x < grid.size(); x++) {
            if (grid[x][y] == water) {
                numWater++;
            }
        }
    }

    cout << "numWater: " << numWater << endl;

    for (int y = 0; y < grid[0].size(); y++) {
        bool record = false;
        for (int x = 0; x < grid.size(); x++) {
            if (grid[x][y] == water) {
                bool stagnant = true;
                for (Coord loc : {Coord {x + 1, y}, Coord {x - 1, y }, Coord { x, y - 1}, Coord {x, y + 1},
                                  Coord {x - 1, y -1}, Coord {x - 1, y + 1 }, Coord { x + 1, y - 1}, Coord {x + 1, y + 1} }) {
                    if (loc.x >= 0 && loc.x <= grid.size() - 1 && loc.y >= 0 && loc.y <= grid[0].size() - 1 && grid[loc.x][loc.y] == sand) {
                        stagnant = false;
                        break;
                    }
                }

                if (stagnant) {
                    grid[x][y] = stagnantWater;
                }
            }
        }
    }

    for (int y = 0; y < grid[0].size(); y++) {
        bool record = false;
        for (int x = 0; x < grid.size(); x++) {
            if (record == false && grid[x][y] == stagnantWater) {
                record = true;
            }
            if (record && grid[x][y] == clay) {
                record = false;
            }

            if (record && grid[x][y] == water) {
                grid[x][y] = stagnantWater;
            }
        }
    }

    for (int y = 0; y < grid[0].size(); y++) {
        bool record = false;
        for (int x = grid.size() - 1; x >= 0; x--) {

            if (record == false && grid[x][y] == stagnantWater) {
                record = true;
            }
            if (record && grid[x][y] == clay) {
                record = false;
            }

            if (record && grid[x][y] == water) {
                grid[x][y] = stagnantWater;
            }

        }
    }

    // print grid
    for (int iy = 0; iy < grid[0].size(); iy++) {
        for (int ix = 0; ix < grid.size(); ix++) {
            if (grid[ix][iy] == sand) {
                cout << '.';
            } else if (grid[ix][iy] == water) {
                cout << '|';
            } else if (grid[ix][iy] == stagnantWater) {
                cout << '~';
            } else {
                cout << '#';
            }
        }
        cout << endl;
    } 

    int numStagWater = 0;
    for (int y = 0; y < grid[0].size(); y++) {
        for (int x = 0; x < grid.size(); x++) {
            if (grid[x][y] == stagnantWater) {
                numStagWater++;
            }
        }
    }
    cout << "num stagnantWater: " << numStagWater << endl;
    return 0;
}