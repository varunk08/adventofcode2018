#include <bits/stdc++.h>

using namespace std;
struct Point {
    int x; int y; int z; int w;
};
int ManDist(const Point& A, const Point& B) {
    return abs(A.x - B.x) + abs(A.y - B.y) + abs(A.z - B.z) + abs(A.w - B.w);
}

int main(int argc, char** argv) {
    vector<vector<Point>> clusters;
    ifstream inFile("day25Input.txt");
    if (inFile.is_open()) {
        string line;
        while(getline(inFile, line)) {
            Point point;
            int begin = 0; int end = line.find(',');
            point.x = stoi(line.substr(begin, end - begin )); 
            begin = end + 1; end = line.find(',', begin);
            point.y = stoi(line.substr(begin, end - begin));
            begin = end + 1; end = line.find(',', begin);
            point.z = stoi(line.substr(begin, end - begin)); 
            begin = end + 1;
            point.w = stoi(line.substr(begin)); 
            //cout << point.x << " "; cout << point.y << " "; cout << point.z << " " << point.w << endl;

                bool addedToCluster = false;
                for (vector<Point>& cluster : clusters) {
                    for (Point& pt : cluster) {
                        if (ManDist(pt, point) <= 3) {
                            addedToCluster = true;
                            break;
                        }
                    }
                    if (addedToCluster) {
                        cluster.push_back(point);
                        break;
                    }
                }

                if (addedToCluster == false) {
                    vector<Point> cluster;
                    cluster.push_back(point);
                    clusters.push_back(cluster);
                }
        }
    }

    for (auto cluster : clusters) {
        for (auto point : cluster) {
            cout << point.x << " "; cout << point.y << " "; cout << point.z << " " << point.w << endl;
        }
        cout << endl;
    }
    cout << "Num clusters: " << clusters.size() << endl;
    return 0;
}