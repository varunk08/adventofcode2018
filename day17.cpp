#include <limits>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

struct Range {
    int startX; int endX; int startY; int endY;

    friend ostream& operator<<( ostream& os, const Range& r)  {
        os << "(" << r.startX << ", " << r.endX << ") (" << r.startY << ", " << r.endY << ")";
        return os;
    }
};
enum Object {
    sand,
    clay,
    water
};


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
    cout << "[Main] x: " << minX << " " <<maxX << endl;
    cout << "[Main] y: " << minY << " " <<maxY << endl;

    // +2 for extra space on either extremities
    vector<vector<Object>> grid(maxX - minX + 3, vector<Object>(maxY - minY + 3, sand));

    for (int y = 0; y < grid[0].size(); y++) {
        for (int x = 0; x < grid.size(); x++) {
            cout << grid[x][y];
        }
        cout << endl;
    }

return 0;+++++++++++++++++++++++++++++++++++++++
    // fill the grid with the ranges
    for (auto range : ranges) {
        cout << range << endl;

        for (int y = range.startY - minY + 1; y <= range.endY - minY + 1; y++) {
            for (int x = range.startX - minX + 1; x <= range.endX - minX + 1; x++) {
                cout << x << " " << y << endl;
                grid[x][y] = clay;
            }
        }
    }
    return 0;
    // fill with stagnant water
    for (int y = 0; y < grid.size(); y++) {
        bool fill = false;
        for (int x = 0; x < grid[x].size(); x++) {
            if (grid[x][y] == clay) {
                fill = !fill;
                continue;
            }
            if (fill) {
                grid[x][y] = water;
            }
        }
    }

    for (int y = 0; y < grid.size(); y++) {
        for (int x = 0; x < grid[x].size(); x++) {
            cout << grid[x][y];
        }
        cout <<endl;
    }


    return 0;
}