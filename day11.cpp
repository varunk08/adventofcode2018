#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <unordered_map>

using namespace std;

struct CellProps {
    int maxPower;
    int size;
};



int SumOfCells(vector<vector<int>>& grid, int x, int y, int gridSize = 3) {
    int sum = 0;
    if ((x + gridSize - 1) >= size(grid) ||
        (y + gridSize - 1) >= size(grid[0])) {
            return sum;
    }

    for (int i = y; i < y + gridSize; i++) {
        for (int j = x; j < x + gridSize; j++) {
            sum += grid[j][i];
        }
    }
    return sum;
}

int SumOfCellsDynamic(vector<vector<int>>& grid, int x, int y, int size, vector<int>& prevSizeScore) {
    if (size == 1) {
        return grid[x][y];
    }

    if (prevSizeScore[size - 1] == numeric_limits<int>::min()) {
        int score = SumOfCellsDynamic(grid, x, y, size - 1, prevSizeScore);
        for (int j = y; j < y + size; j++) {
            score += grid[x + size -1][j];
        }

        for (int i = x; i < x + size; i++) {
            score += grid[i][y + size -1];
        }
        score -= grid[x + size - 1][y + size -1]; // this is added twice.

        prevSizeScore[size - 1] = score;
    }
    
    return prevSizeScore[size - 1];
}

void FillGrid(vector<vector<int>>& grid, int gridSerialNum) {
    for (int y = 0; y < size(grid); ++y) {
        for (int x = 0; x < size(grid[y]); ++x) {
            int rackId = (x + 1) + 10;
            int powerLevel = rackId * (y + 1) + gridSerialNum;
            powerLevel *= rackId;
            powerLevel = (powerLevel / 100) % 10;
            powerLevel -= 5;
            grid[x][y] = powerLevel;
        }
    }
}

CellProps FindMaxScore(vector<vector<int>>& grid, int x, int y) {
    CellProps cellProps = {numeric_limits<int>::min(), 0};
    vector<int> prevSizeScore(grid.size(), numeric_limits<int>::min());    
    
    for (int size = 1; size <= (grid.size() - x) && size <= (grid.size() - y); ++size) {
        int curScore = SumOfCellsDynamic(grid, x, y, size, prevSizeScore);
        if (cellProps.maxPower < curScore) {
            cellProps.maxPower = curScore;
            cellProps.size = size;
            //cout << curScore << ": " << size << endl;
        }
    }

    return cellProps;
}

int main(int argc, char** argv) {
    const int numCells = 300;
    const int gridSerialNum = 8141;
    vector<vector<int>> grid(numCells, vector<int>(numCells, 0));

    FillGrid(grid, gridSerialNum);
    //cout << SumOfCells(grid, 20, 60) << endl;
    int maxSum = numeric_limits<int>::min();

    // part 1
    for (int y = 0; y < size(grid); y++) {
        for (int x = 0; x < size(grid[y]); x++) {
            int curSum = SumOfCells(grid, x, y);
            if (maxSum < curSum) {
                maxSum = curSum;
            }           
        }
    }
    
    cout << "max: " << maxSum << endl;
    

    vector<vector<CellProps>> scoreGrid(numCells, vector<CellProps>(numCells, {0, 0}));

    for (int y = 0; y < size(grid); y++) {
        for (int x = 0; x < size(grid[y]); x++) {
            scoreGrid[x][y] = FindMaxScore(grid, x, y);
        }
    }
    
    struct MaxCell {
        int x;
        int y;
        int size;
    } maxCell = {0,0,0};
    int maxScore = numeric_limits<int>::min();

    for (int y = 0; y < size(scoreGrid); y++) {
        for (int x = 0; x < size(scoreGrid[y]); x++) {
            if (maxScore < scoreGrid[x][y].maxPower) {
                maxScore = scoreGrid[x][y].maxPower;
                maxCell.x = x +1;
                maxCell.y = y +1;
                maxCell.size = scoreGrid[x][y].size;
            }
        }
    }

cout << maxScore << " " << maxCell.x << ", " << maxCell.y << ", " << maxCell.size << endl;

    return 0;
}