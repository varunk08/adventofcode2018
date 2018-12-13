#include <iostream>
#include <fstream>
#include <list>
#include <limits>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

using NumType = unsigned int;

int main(int argc, char** argv) {
    
    int numPlayers      = 418;
    int lastMarbleScore = 71339;

    vector<NumType> score(numPlayers, 0);
    list<int> circle;
    circle.push_front(0);
    list<int>::iterator curMarble = circle.begin();
    int curPlayer = 0;

    for (int i = 1; i <= lastMarbleScore; ++i) {
        // get iter to insert
        list<int>::iterator cwIter = curMarble;
        cwIter++;
        if (cwIter == circle.end()) {
            cwIter = circle.begin();
        }
        cwIter++;
        if (cwIter == circle.end()) {
            cwIter = circle.begin();
        }

        if ((i % 23) != 0) {
            curMarble = circle.insert(cwIter, i);
        } else {
            // get rev iterator to delete
            list<int>::reverse_iterator rIter = list<int>::reverse_iterator(++curMarble);
            int numReverseSteps = 7;
            while (numReverseSteps--) {
                rIter++;
                if (rIter == circle.rend()) {
                    rIter = circle.rbegin();
                }
            }

            score[i % numPlayers] += (i + *rIter);

            // update current marble
            auto tmp = circle.cbegin();
            if (rIter == circle.rend()) {
                tmp = (rIter).base();
            } else {
                tmp = (++rIter).base();
            }

            curMarble = circle.erase(tmp);
            if (curMarble == circle.end()) {
                curMarble = circle.begin();
            }
        }            
        
        /*
        cout << curPlayer << ": ";
        for (auto item : circle) {
            if (item == *curMarble) {
                cout <<"(" << item << ") ";    
            } else {
                cout << item << " ";
            }
        }
        cout << "  [" <<size(circle) <<"]" << endl;       
        */
        //cout << size(circle) << endl;        
    }

    NumType maxScore = numeric_limits<NumType>::min();
    for (auto s : score) {
        maxScore = max(maxScore, s);
    }
    //cout << std::fixed << std::setprecision(2);
    cout << "Max: " << maxScore << endl;
    return 0;
}