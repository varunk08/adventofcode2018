#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <algorithm>
#include <limits>
#include <utility>
#include "day10Input.h"
#include "bitmap_image.hpp"

using namespace std;

void PrintPoint(const Point& pt) {
    cout << "(" << pt.x <<", " << pt.y << ")" << endl;
}

void PrintGrid(vector<vector<bool>>& grid) {
    for (auto vec : grid) {
        for (auto i : vec) {
            if (i == true) {
                cout << " # ";
            } else {
                cout << " . ";
            }
        }
        cout << endl;
    }
}

int main(int argc, char** argv) {

    vector<Point>& positions = positionsTest;
    vector<Velocity>& velocities = velocitiesTest;

    if (size(positions) != size(velocities)) {
        cout << "Error!" << endl;
        return -1;
    }

    // find max and min bounds
    Point minPoint = {numeric_limits<int>::max(),numeric_limits<int>::max()};
    Point maxPoint = {numeric_limits<int>::min(), numeric_limits<int>::min()};
    for (auto pt : positions) {
        minPoint.x = min(minPoint.x, pt.x);
        minPoint.y = min(minPoint.y, pt.y);

        maxPoint.x = max(maxPoint.x, pt.x);
        maxPoint.y = max(maxPoint.y, pt.y);
    }

    PrintPoint(minPoint);
    PrintPoint(maxPoint);
    Point inputDim = {
        abs(maxPoint.x - minPoint.x),
        abs(maxPoint.y - minPoint.y)
    };
    cout << endl;

    int width = inputDim.x;
    int height = inputDim.y;

    bitmap_image day10Image(width, height);
    day10Image.clear(125);

    vector<vector<bool>> grid(width + 1, vector<bool>(height + 1, false));
    // translate
    Point transToOrigin = { -minPoint.x, -minPoint.y};
    for(Point& pt : positions) {
        pt += transToOrigin;
        //PrintPoint(pt);
        grid[pt.x][pt.y] = 1;
    }

    //vector<std::pair<float, float>> scaledPts(size(positions));
    // scale the points
    Point scale = maxPoint;
    for(Point& pt : positions) {
        //pt /= scale;
        //PrintPoint(pt);
        //grid[pt.x][pt.y] = 1;
        //scaledPts.push_back({ (pt.x / static_cast<float>(inputDim.x)) * width, (pt.y / static_cast<float>(inputDim.y)) * height });
        //cout << "(" << (pt.x / static_cast<float>(inputDim.x)) * width << ", " << (pt.y / static_cast<float>(inputDim.y)) * height << ")" << endl;
    }

/*
    int numIter = 20;

    day10Image.pen_color(255, 0, 0);
    while(numIter--) {        
        for (int i = 0; i < size(scaledPts); i++) {
            //scaledPts[i].first = scaledPts[i].first + velocities[i].x;
            //scaledPts[i].second = scaledPts[i].second + velocities[i].y;
            //day10Image.plot_pen_pixel(scaledPts[i].first, scaledPts[i].second);

            positions[i].x += velocities[i].x * 10;
            positions[i].y +=  velocities[i].y * 10;
            //day10Image.plot_pen_pixel(positions[i].x, positions[i].y);
            day10Image.fill_circle(positions[i].x, positions[i].y, 5);
        }
        string filename = "day10_image_" + to_string(numIter) + ".bmp";
        day10Image.image().save_image(filename.c_str());
    }
*/

    int numIter = 10;
    cout << "rendering " << endl;
    while(numIter--) {
        day10Image.clear();
        // clear the grid;
        for (int i = 0; i < size(grid); i++) {
            for (int j = 0; j < size(grid[i]); j++) {
                grid[i][j] = false;
            }
        }

        // update the positions
        for (int i = 0; i < size(positions); ++i) {
            positions[i] += velocities[i];
            if (positions[i].x >= 0 && 
                positions[i].x < width &&
                positions[i].y >= 0 &&
                positions[i].y < height) {
                    grid[positions[i].x][positions[i].y] = true;
                    day10Image.set_pixel(positions[i].x, positions[i].y, 255,0,0);
                }            
        }       

        string filename = "day10_image_" + to_string(numIter) + ".bmp";
        day10Image.save_image(filename.c_str());       

        // render grid
        //PrintGrid(grid);
    }
    
    return 0;
}