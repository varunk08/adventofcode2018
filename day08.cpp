#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include "day08Input.h"

using namespace std;

struct Node {
    struct Header {
        int numChildren;
        int numMetadata;
    } header;

    int key;
    vector<int> childKeys;
    vector<int> metadata;
};

/*vector<Node> ConstructChildNodes(int start, int* pEnd, int numChildren, vector<int>& data) {
    vector<int> childKeys;
    if (numChildren == 0) {
        return childKeys;
    }
    else {
        int numCurChildren = data[start];

        return ConstructChildNodes()
    }
}
*/

int CalculateMetadataSum(int* pCurIdx, vector<int>& data) {
    int value = 0;
    int metadataSum = 0;
    int& idx = *pCurIdx;
    int numChildren = data[idx++];
    int numMetadata = data[idx++];

    vector<int> childValue(numChildren, 0);
    for (int i = 0; i < numChildren; ++i) {
        //metadataSum += CalculateMetadataSum(pCurIdx, data);
        childValue[i] = CalculateMetadataSum(pCurIdx, data);
    }

    vector<int> metadataValue(numMetadata, 0);
    for (int j = 0; j < numMetadata; ++j) {
        //metadataSum += data[idx++];
        metadataValue[j] = data[idx++];
    }

    if (numChildren != 0) {
        // 1 = 0th child
        // 2 = 1st child etc
        for (auto meta : metadataValue) {
            if (meta <= numChildren) {
                value += childValue[meta - 1];
            }
        }
    } else {
        for (auto metadata : metadataValue) {
            value += metadata;
        }
    }
    //return metadataSum;
    return value;
}

int main(int argc, char** argv) {

    vector<int> data = {
        2, 3, 0, 3, 10, 11, 12, 1, 1, 0, 1, 99, 2, 1, 1, 2,
    };

    cout << "size: " << size(data) << endl;
    int idx = 0;
    cout << "Sum: " << CalculateMetadataSum(&idx, data) << endl;

    cout << "size: " << size(day08Input) << endl;
    idx = 0;
    cout << "Sum: " << CalculateMetadataSum(&idx, day08Input) << endl;
    return 0;
}