#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <unordered_set>

using namespace std;

void PrintGrid(vector< vector<int>>& grid) {
    for (auto vec : grid) {
        for (auto num : vec) {
            cout << num << " ";
        }
        cout << endl;
    }
}

struct Coord {
    int x; int y;
};

int main(int argc, char** argv) {
    vector<Coord> points = {
        {84, 212},
        {168, 116},
        {195, 339},
        {110, 86},
        {303, 244},
        {228, 338},
        {151, 295},
        {115, 49},
        {161, 98},
        {60, 197},
        {40, 55},
        {55, 322},
        {148, 82},
        {86, 349},
        {145, 295},
        {243, 281},
        {91, 343},
        {280, 50},
        {149, 129},
        {174, 119},
        {170, 44},
        {296, 148},
        {152, 160},
        {115, 251},
        {266, 281},
        {269, 285},
        {109, 242},
        {136, 241},
        {236, 249},
        {338, 245},
        {71, 101},
        {254, 327},
        {208, 231},
        {289, 184},
        {282, 158},
        {352, 51},
        {326, 230},
        {88, 240},
        {292, 342},
        {352, 189},
        {231, 141},
        {280, 350},
        {296, 185},
        {226, 252},
        {172, 235},
        {137, 161},
        {207, 90},
        {101, 133},
        {156, 234},
        {241, 185},
    };

    vector< Coord> pointsTest = {
{1, 1},
{1, 6},
{8, 3},
{3, 4},
{5, 5},
{8, 9}
    };
//points.clear();
//points = pointsTest;

    Coord bottomRight = {numeric_limits<int>::min(), numeric_limits<int>::min()};
    for (auto coord : points) {
        bottomRight.x = max(bottomRight.x, coord.x);
        bottomRight.y = max(bottomRight.y, coord.y);
    }
    cout << bottomRight.x << ", " << bottomRight.y << endl;
    int topBand = 0;
    int leftBand = 0;
    int rightBand = bottomRight.x + 2;
    int bottomBand = bottomRight.y + 2;

    vector< vector <int> > grid(rightBand + 1, vector<int>(bottomBand + 1, -1));

    for (int i = 0; i < size(points); i++) {
        points[i].x++; points[i].y++;
    }

    cout << "Num points: " << size(points) << endl;
    for (int x = leftBand; x <= rightBand; x++) {
        for (int y = topBand; y <= bottomBand; y++) {
            int manDist = numeric_limits<int>::max();
            int minPointIdx = -1;
            int numPoints = 0;
            for (int i = 0; i < size(points); i++) {
                int curDist = abs(x - points[i].x) + abs(y - points[i].y);
                if (curDist < manDist) {
                    manDist = curDist;
                    minPointIdx = i;
                }
            }

            for (int i = 0; i < size(points); i++) {
                int curDist = abs(x - points[i].x) + abs(y - points[i].y);
                if (curDist == manDist) {
                    numPoints++;
                }
            }
            
            if (numPoints == 1) {
                grid[x][y] = minPointIdx;
            } else {
                grid[x][y] = -2;
            }            
        }
    }
    
    //PrintGrid(grid);
    
    vector<int> counts(size(points), 0);
    unordered_set<int> validPoints;
    for(int i = 0; i < size(points); i++) {
        validPoints.emplace(i);
    }

    for(int x = 0; x <= rightBand; x++) {
        validPoints.erase(grid[x][topBand]);
        validPoints.erase(grid[x][bottomBand]);
    }

    for(int y = 0; y <= bottomBand; y++) {
        validPoints.erase(grid[leftBand][y]);
        validPoints.erase(grid[rightBand][y]);
    }

    int maxArea = numeric_limits<int>::min();

    for(auto it = validPoints.begin(); it != validPoints.end(); ++it) {
        cout << *it << endl;
        int pointArea = 0;
        for (int x = leftBand + 1; x < rightBand; x++) {
            for (int y = topBand + 1; y < bottomBand; y++) {
                if (grid[x][y] == *it)  {
                    pointArea++;                    
                }
            }
        }

        maxArea = max(pointArea, maxArea);
    }
    cout << "max: " << maxArea << endl;
    cout << size(validPoints) << endl;

    int distBoundArea = 0;
   
        for (int x = leftBand + 1; x < rightBand; x++) {
            for (int y = topBand + 1; y < bottomBand; y++) {
                if (validPoints.find(grid[x][y]) != validPoints.end()) {
                    int distSum = 0;
                    for (auto pt : points) {
                        distSum += (abs(pt.x - x) + abs(pt.y - y));
                        if (distSum >= 10000) {
                            break;
                        }
                    }

                    if (distSum < 10000) {
                        distBoundArea++;
                    }
                }
            }
        }

    cout << "area: " << distBoundArea + size(validPoints) << endl;

        distBoundArea = 0;
        for (int x = leftBand + 1; x < rightBand; x++) {
            for (int y = topBand + 1; y < bottomBand; y++) {
                int distSum = 0;
                    for (auto pt : points) {
                        int dist = (abs(pt.x - x) + abs(pt.y - y));
                        distSum += dist == 0 ? 1 : dist;
                        if (distSum >= 10000) {
                            break;
                        }
                    }

                    if (distSum < 10000) {
                        distBoundArea++;
                    }
            }
        }
    cout << "area: " << distBoundArea << endl;

    return 0;
}