#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Region {
    Region(int x) :  erosion(x) {}
    int erosion;
};

int main(int argc, char** argv) {
    int depth = 8103;
    int targetX = 9;
    int targetY = 758; // x
    vector<vector<Region>> cave(targetX + 1, vector<Region>(targetY + 1, {0}));
    
    for (int x = 0; x <= targetX; x++) {
        for (int y = 0; y <= targetY; y++) {
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

    return 0;
}