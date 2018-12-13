#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <limits>
#include <algorithm>

using namespace std;

enum TrackType {
    Vertical,
    Turn,
    Intersection
};

struct Cell {
    bool containsCart;

};

int main(int argc, char** argv) {
    ifstream inFile("day13Input.txt");
    string line;
    vector<vector<Cell>> grid;

    if (inFile.is_open()) {
        unsigned long long maxW = 0;
        while(getline(inFile, line)) {
            maxW = max(line.size(), maxW);
        }
    }
    
    return 0;
}