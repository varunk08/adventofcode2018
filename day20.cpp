#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stack>

using namespace std;

struct Node {
    Node(char _dir, int _doorDist, int _x, int _y)
    :
    dir(_dir),
    doorDist(_doorDist),
    x(_x),
    y(_y)  {
        children.resize(3);
        for (int i = 0; i < children.size(); i++) {
            children[i].resize(0);
        }
    }

    char dir;
    int doorDist;
    int x; int y;
    int curChildIdx = 0;
    vector<vector<Node*>> children;
};

struct pair_hash {
    inline std::size_t operator()(const std::pair<int,int> & v) const {
        return v.first*31+v.second;
    }
};

char specialChars[] = { '(', ')', '|', '^' };

void GenCombosHelper(vector<string>& combos, string chars, int i) {
    if (i == chars.length() - 1)  {
        combos.push_back(chars);
        return;
    }

    for (int j = i; j < chars.length(); j++) {
        swap(chars[i], chars[j]);
        GenCombosHelper(combos, chars, i + 1);
        swap(chars[i], chars[j]);
    }
}

void GenerateCombinations(vector<string>& combos, string chars) {
    GenCombosHelper(combos, chars, 0);
}

void PrintMaze(vector<Node*> nodes) {
    for (Node* pNode : nodes) {
        cout << pNode->dir;
        for (auto vec : pNode->children) {
            if (vec.size()) {
                cout << "[";
                PrintMaze(vec);
                cout << "]";
            }
        }
    }
}

void CalcLongestPath(vector<Node*> nodes, int& pathLength) {
    for (auto pNode : nodes) {
        int biggestChild = 0;
        for (auto vec : pNode->children) {
            if (vec.size()) {
                int curChildSize = vec.size();
                CalcLongestPath(vec, curChildSize);
                biggestChild = max(biggestChild, curChildSize);
            }
        }
        pathLength += biggestChild;
    }
}

void CalcPathLengthAtLeast1k(vector<Node*> nodes,int& numGtr1k, int atLeast) {
    for (auto pNode : nodes) {
        for (auto vec : pNode->children) {         
            if (vec.size() > 0) {
                for (auto pNode : vec) {
                    //cout << pNode->doorDist << " ";
                    if (pNode->doorDist >= atLeast) {
                        numGtr1k++;
                    }
                }

                CalcPathLengthAtLeast1k(vec, numGtr1k, atLeast);
            }
        }
    }   
}

int main(int argc, char** argv) {
    ifstream inFile("day20Input.txt");
    vector<string> badCombos;
    GenerateCombinations(badCombos, "NEWS");
    GenerateCombinations(badCombos, "EEWW");
    GenerateCombinations(badCombos, "NNSS");
    vector<string> twoCombos;
    GenerateCombinations(twoCombos, "NS");
    GenerateCombinations(twoCombos, "EW");   

    string line;

    unordered_map<string, int> numValidPts;
    for(auto str : badCombos) {
        unordered_set<pair<int,int>, pair_hash> pts;
        
        int stX = 0; int stY = 0;
        pts.emplace(make_pair(stX, stY));

        for (auto ch : str) {
            switch (ch) {
                case 'N': stY++; break;
                case 'S': stY--; break;
                case 'E': stX++; break;
                case 'W': stX--; break;
            }
            pts.emplace(make_pair(stX, stY));
        }

        numValidPts.emplace(str, pts.size() - 1);
        //cout << str << " " << pts.size() - 1 << endl;
    }

    cout << "[Main] num bad combos: " << badCombos.size() << endl;
    cout << "[Main] num set pts: "    << numValidPts.size() << endl;
    cout << "[Main] num bad combos: " << twoCombos.size() << endl;

    if (inFile.is_open()) {        
        getline(inFile, line);
        // const bool removeThreeCombos = false;
        // const bool removeTwoCombos = false;

        // while (removeTwoCombos) {
        //     int numCombos = twoCombos.size();
        //     for (auto str : twoCombos) {
        //         int badPos = line.find(str);
        //         if (badPos != string::npos) {
        //             line.erase(badPos + 1, size(str) - 1);
        //         } else {
        //             numCombos--;
        //         }
        //     }

        //     if (numCombos == 0) {
        //         break;
        //     }
        // }

        // while (removeThreeCombos) {
        //     int numCombos = badCombos.size();
        //     for (auto str : badCombos) {
        //         auto it = numValidPts.find(str);
        //         if (it == numValidPts.end()) {
        //             cout << "something wrong" << endl;
        //         }

        //         int badPos = line.find(str);
        //         if (badPos != string::npos) {
        //             line.erase(badPos + it->second, size(str) - it->second);
        //         } else {
        //             numCombos--;
        //         }
        //     }

        //     if (numCombos == 0) {
        //         break;
        //     }
        // }         
    }

    line.erase(0, 1); // erase '^'
    line.erase(line.size() - 1, 1); // erase '$'
    
    stack<Node*> branchRoots;
    vector<Node*> nodes;
    unordered_map<pair<int,int>, int, pair_hash> doorsDistMap;

    Node* pNode = new Node('x', 0, 0, 0);
    nodes.push_back(pNode);
    branchRoots.push(nodes.at(0));

    for(auto ch : line) {
        //cout << ch;

        if (ch != '(' && ch != ')' && ch != '|') { // it is a direction
            Node* pRoot = branchRoots.top();
            int doorDist = pRoot->doorDist;
            int stX = pRoot->x ; int stY = pRoot->y;

            if (pRoot->children[pRoot->curChildIdx].size() > 0) {
                auto pPrevCode = pRoot->children[pRoot->curChildIdx].at(pRoot->children[pRoot->curChildIdx].size() - 1);
                stX = pPrevCode->x;
                stY = pPrevCode->y;
                doorDist = pPrevCode->doorDist;
            }

            int x = stX; int y = stY;
            switch (ch) {
                case 'N': y++; break;
                case 'S': y--; break;
                case 'E': x++; break;
                case 'W': x--; break;
            }
            
            auto it = doorsDistMap.find(make_pair(x, y));
            if (it == doorsDistMap.end()) {
                doorsDistMap.emplace(make_pair(x, y), ++doorDist);
                Node* pNode = new Node(ch, doorDist, x, y);
                pRoot->children[pRoot->curChildIdx].push_back(pNode);                
            } else {
                auto prevIt = doorsDistMap.find(make_pair(stX, stY));
                if (prevIt != doorsDistMap.end()) {
                    prevIt->second = it->second + 1;
                } else {
                    cout << "previous node not found" << endl;
                }
            }          
        }

        if (ch == '(') { // new branch
            Node* pRoot = branchRoots.top();
            if (pRoot->children[pRoot->curChildIdx].size()) { // handle cases '((' '(|('
                Node* pBranchRoot = pRoot->children[pRoot->curChildIdx].at(pRoot->children[pRoot->curChildIdx].size() - 1);
                branchRoots.push(pBranchRoot); 
            }
        } else if (ch == '|') { // same branch but different child
            Node* pRoot = branchRoots.top();
            pRoot->curChildIdx++;
        } else if (ch == ')') { // branch is done
            branchRoots.pop();
        }
    }

    cout << endl;
    cout << "[Main] after insertion " << branchRoots.size() << endl;

    //PrintMaze(nodes);
    int pathLength = 0;    
    CalcLongestPath(nodes, pathLength);
    cout << "biggestChild: " << pathLength << endl; 

    int numGtr1k = 0;

    CalcPathLengthAtLeast1k(nodes, numGtr1k, 1000);
    cout << endl;
    cout << "numGtr1k: " << numGtr1k << endl;

    int numLongPaths = 0;
    for (auto it = doorsDistMap.begin(); it != doorsDistMap.end(); it++) {
        if (it->second >= 999) {
            numLongPaths++;
        }
    }

    cout << "numLongPaths: " << numLongPaths<< endl;
    

    return 0;
}