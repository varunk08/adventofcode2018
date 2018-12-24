#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include <unordered_map>
#include <limits>

using namespace std;
struct Point {
    long int x; long int y; long int z;
};
struct Bot {
    int x; int y; int z;
    unsigned int radius;
};

int Distance(vector<Bot>& bots, int i, int j) {
    return sqrt(pow(bots[i].x - bots[j].x, 2) + pow(bots[i].y - bots[j].y, 2) + pow(bots[i].z - bots[j].z, 2));
}

int main(int argc, char** argv) {
    ifstream inFile("day23Input.txt");
    vector<Bot> bots;

    if(inFile.is_open()) {
        string line;
        while(getline(inFile, line)) {
            Bot bot = {};
            int begin = line.find('<'); begin++;
            int end = line.find(',');
            bot.x = stoi(line.substr(begin, end - begin));
            begin = end + 1;
            end = line.find(',', begin);
            bot.y = stoi(line.substr(begin, end - begin));
            begin = end + 1;
            end = line.find('>');
            bot.z = stoi(line.substr(begin, end - begin));
            begin = line.find('r') + 2;
            bot.radius = stoi(line.substr(begin));
            bots.push_back(bot);
        }
    }

    cout << "Num bots: " << bots.size() << endl;
    sort (bots.begin(), bots.end(), [](const Bot& A, const Bot& B) {
        return A.radius > B.radius;
    });

    Bot& largest = bots[0];
    int maxRadius = bots[0].radius;
    cout << "largest r " << bots[0].radius << endl;

    int numInRange = 0;
    for (auto bot : bots) {
        int diffX = abs(largest.x - bot.x);
        int diffY = abs(largest.y - bot.y);
        int diffZ = abs(largest.z - bot.z);

        if ((diffX + diffY + diffZ) <= maxRadius) {
            numInRange++;
        }
    }
    cout << "Num in range: " << numInRange << endl; 

    // part 2
    /*int maxInsideCount = 0;
    Point maxPoint = {numeric_limits<int>::max(), numeric_limits<int>::max(), numeric_limits<int>::max()};
    for (int i = 0; i < bots.size(); i++) {
        for (int j = 0; j < bots.size(); j++) {
            if (i == j) continue;
            if (Distance(bots, i, j) <= bots[i].radius + bots[j].radius) {
                Point midPoint = {
                    (bots[i].x + bots[j].x) / 2,
                    (bots[i].y + bots[j].y) / 2,
                    (bots[i].z + bots[j].z) / 2
                };
                int insideCount = 0;
                
                for (int n = 0; n < bots.size(); n++) {
                    if (sqrt(pow(bots[n].x - midPoint.x, 2) +
                             pow(bots[n].y - midPoint.y, 2) +
                             pow(bots[n].z - midPoint.z, 2)) <= bots[n].radius) {
                        insideCount++;
                    }
                }

                if (insideCount == maxInsideCount) {
                    if (abs(maxPoint.x) + abs(maxPoint.y) + abs(maxPoint.z) > 
                        abs(midPoint.x) + abs(midPoint.y) + abs(midPoint.z)) {
                        maxPoint = midPoint;
                        maxInsideCount = insideCount;
                    }
                } else if (insideCount > maxInsideCount) {
                    maxPoint = midPoint;
                    maxInsideCount = insideCount;
                    cout << "new: " << midPoint.x << " " << midPoint.y << " " << midPoint.z << " "
                         << "sum: " << abs(midPoint.x) + abs(midPoint.y) + abs(midPoint.z) << endl;
                }
            }
        }
    }

    cout << "max: " << maxPoint.x << " " << maxPoint.y << " " << maxPoint.z << " " << endl;
    cout << "sum: " << abs(maxPoint.x) + abs(maxPoint.y) + abs(maxPoint.z) << endl;
    */
    /* trying out solution from https://github.com/fhinkel/AdventOfCode2018/blob/master/day23.js */

    int minX = numeric_limits<int>::max();
    int maxX = numeric_limits<int>::min();
    int minY = numeric_limits<int>::max();
    int maxY = numeric_limits<int>::min();
    int minZ = numeric_limits<int>::max();
    int maxZ = numeric_limits<int>::min();
    
    for (auto bot : bots) {
        if (bot.x < minX) {
            minX = bot.x;
        }
        if (bot.x > maxX) {
            maxX = bot.x;
        }
        if (bot.y < minY) {
            minY = bot.y;
        }
        if (bot.y > maxY) {
            maxY = bot.y;
        }
        if (bot.z < minZ) {
            minZ = bot.z;
        }
        if (bot.z > maxZ) {
            maxZ = bot.z;
        }
    }
    int gridSize = maxX - minX;
    Point bestGrid = {numeric_limits<int>::max(), numeric_limits<int>::max(), numeric_limits<int>::max()};   
    while (gridSize > 0) {
        int maxCount = 0;
        cout << gridSize << endl;
        for (int x = minX; x < maxX + 1; x+= gridSize) {
            for (int y = minY; y < maxY + 1; y+=gridSize) {
                for (int z = minZ; z < maxZ + 1; z+=gridSize) {
                    int count = 0;
                    
                    for (auto bot : bots) {
                        int dist = abs(x - bot.x) + abs(y - bot.y) + abs(z - bot.z);
                        if (bot.radius >= dist) {
                            count++;
                        }
                    }

                    if (maxCount < count) {
                        maxCount = count;
                        bestGrid = {x, y, z};
                    } else if (maxCount == count) {
                        if (abs(bestGrid.x) + abs(bestGrid.y) + abs(bestGrid.z) >  abs(x) + abs(y) + abs(z)) {
                                bestGrid = {x, y, z};
                        }
                    }
                }
            }
        }

        minX = bestGrid.x - gridSize;
        maxX = bestGrid.x + gridSize;
        minY = bestGrid.y - gridSize;
        maxY = bestGrid.y + gridSize;
        minZ = bestGrid.z - gridSize;
        maxZ = bestGrid.z + gridSize;
        cout << minX << " " << maxX << endl;
        gridSize = gridSize / 2;
    }

    cout << "best grid: " << abs(bestGrid.x) + abs(bestGrid.y) + abs(bestGrid.z) << endl;

    return 0;
}