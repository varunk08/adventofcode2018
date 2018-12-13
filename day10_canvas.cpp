#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <algorithm>
#include <limits>
#include <utility>
#include <functional>
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

    if (argc <= 1) {
        return 0;
    }

    long unsigned int numIter = stoi(argv[1]);
    int velFactor = stoi(argv[2]);
    int testInput = stoi(argv[3]);
    vector<Point>& positions     = positionsPuzz;
    vector<Velocity>& velocities = velocitiesPuzz;
    if (testInput == 1) {
        positions  = positionsPuzz;
        velocities = velocitiesPuzz;
    } else if (testInput == 2) {
        positions  = positionsTest;
        velocities = velocitiesTest;
    }
    

    const int GridWidth  = 1200;
    const int GridHeight = 400;    

    // Find median x point
    vector<int> ptDim;
    vector<Point> withinBoundsPos;
    vector<Velocity> withinBoundsVel;
    Point midPoint = {};
  
    cartesian_canvas canvas(GridWidth, GridHeight);
    canvas.pen_width(2);
    
    cout << "rendering " << endl;    

    int leftBound =  - (GridWidth / 2);
    int rightBound =  + (GridWidth / 2);
    int topBound  =  -(GridHeight / 2);
    int botBound =  + (GridHeight / 2);
    
    for (Velocity& vel : velocities) {
        vel.x *= velFactor;
        vel.y *= velFactor;
    }

    while(numIter--) {        
        cout << "\r" << numIter;
        canvas.image().clear(0);
        canvas.pen_color(255,0,0);
        canvas.pen_color(255,255,0);

        for (int i = 0; i < size(positions); ++i) {
            positions[i] += velocities[i];
        }
    
    if (numIter < 100) {
        // find max and min bounds
        Point minPoint = {numeric_limits<int>::max(),numeric_limits<int>::max()};
        Point maxPoint = {numeric_limits<int>::min(), numeric_limits<int>::min()};
        for (auto pt : positions) {
            minPoint.x = min(minPoint.x, pt.x);
            minPoint.y = min(minPoint.y, pt.y);

            maxPoint.x = max(maxPoint.x, pt.x);
            maxPoint.y = max(maxPoint.y, pt.y);
        }

        Point inputDim = { abs(maxPoint.x - minPoint.x), abs(maxPoint.y - minPoint.y)};        
        inputDim.x += (0.3 * inputDim.x);
        inputDim.y += (0.3 * inputDim.y);
        PrintPoint(inputDim);
        ptDim.clear();
        for (auto pt : positions) {
            ptDim.push_back(pt.x);
        }
        sort(ptDim.begin(), ptDim.end(), std::less<int>());
        midPoint.x = ptDim[ptDim.size() / 2];
        ptDim.clear();
        for (auto pt : positions) {
            ptDim.push_back(pt.y);
        }
        sort(ptDim.begin(), ptDim.end(), std::less<int>());

        midPoint.y = ptDim[ptDim.size() / 2];
        float x =0;
        float y =0;
        for (int i = 0; i < size(positions); ++i) {
            x = static_cast<float>(positions[i].x - midPoint.x) / (inputDim.x) * GridWidth;
            y = static_cast<float>(positions[i].y - midPoint.y) / (inputDim.y) * GridHeight;
            canvas.plot_pen_pixel(x, y);
        }

            string filename = "images/day10_image_" + to_string(numIter) + ".bmp";
            canvas.image().save_image(filename.c_str());
        }
    }

    cout << endl << "done" << endl;
    return 0;
}