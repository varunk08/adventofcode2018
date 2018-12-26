#include <cmath>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stack>

using namespace std;

struct Node {
    Node(char _dir) : dir(_dir) {
        children.resize(3);
        for (int i = 0; i < children.size(); i++) {
            children[i].resize(0);
        }
    }

    char dir;
    int curChildIdx = 0;
    vector<vector<Node*>> children;
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

int main(int argc, char** argv) {
    ifstream inFile("day20Input.txt");
    vector<string> badCombos;
    GenerateCombinations(badCombos, "NEWS");
    GenerateCombinations(badCombos, "EEWW");
    GenerateCombinations(badCombos, "NNSS");
    GenerateCombinations(badCombos, "NS");
    GenerateCombinations(badCombos, "EW");
    for (auto str : badCombos) {
        cout << str << endl;
    }
    cout << badCombos.size() << endl;
    string line;
    if (inFile.is_open()) {        
        getline(inFile, line);

        while (true) {
            int numCombos = badCombos.size();
            for (auto str : badCombos) {
                int badPos = line.find(str);
                if (badPos != string::npos) {
                    line.erase(badPos, size(str));
                } else {
                    numCombos--;
                }
            }

            if (numCombos == 0) {
                break;
            }
        }
    }

    cout << line << endl;

    line.erase(0, 1);
    line.erase(line.size() - 1, 1);
    stack<Node*> branchRoots;
    vector<Node*> nodes;
    Node* pNode = new Node('x');
    nodes.push_back(pNode);
    branchRoots.push(nodes.at(0));
 
    for(auto ch : line) {
        //cout << ch;

        if (ch != '(' && ch != ')' && ch != '|') { // it is a direction
            Node* pNode = new Node(ch);
            Node* pRoot = branchRoots.top();
            pRoot->children[pRoot->curChildIdx].push_back(pNode);
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

    return 0;
}