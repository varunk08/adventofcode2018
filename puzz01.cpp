#include <iostream>
#include <vector>
#include <unordered_set>

using namespace std;

int main(int argc, char** argv) {
    vector<int> puzzInput = {+13,-16,+18,+11,+15,+10,-4,-19,+14,-19,-17,-19,-2,+6,+2,+21,+5,+12,-10,-10,+18,-11,+2,+17,-5,+14,+14,-3,-12,+10,+7,-3,+6,-2,+8,-17,-18,-19,-14,+9,-16,-1,-1,+11,-3,+8,-12,+10,+14,+12,-18,+19,-5,+13,+1,+4,+2,+11,-3,-2,+18,+8,-4,+15,-7,-3,+18,+16,+2,-4,-12,-19,-4,+12,+15,-17,+1,-16,-5,+15,+26,+9,+9,-3,-5,+3,+3,-11,-8,-11,-16,+14,+14,+14,+17,+14,+11,-9,+18,-10,-4,-11,-10,-9,+8,+12,-6,-1,-2,+12,-17,-1,+17,-10,+9,+11,+6,+3,+18,+19,+11,-3,+15,-3,-2,+18,+17,+13,+13,-6,+16,+10,+7,-6,-15,-15,-1,+15,+6,-2,-12,+19,-1,+17,+18,-8,-11,-18,+12,-3,-14,+6,-12,+1,-11,-9,+13,-3,-15,-19,-16,-3,+7,+7,-17,-18,+3,-2,-12,-1,+16,+15,+10,+7,-13,+7,+13,+14,+5,+18,-14,-8,-4,+15,-20,-17,-17,+11,-15,-14,-10,-1,+5,-11,+18,-4,-19,+15,-8,+14,-20,-12,+10,-11,-12,+16,-7,-7,+4,-18,-8,+11,+13,-6,+24,+8,+4,-19,+11,+15,+3,-17,+30,+9,+10,+6,-2,+3,+14,+8,-3,-12,+1,+5,-8,+11,+10,+16,+6,+16,+1,+21,+19,+18,+18,+15,+15,+8,+2,-6,-16,+5,-10,+18,-12,+16,+6,+18,-6,-2,+5,-14,-17,-14,-13,-7,+16,-8,-11,-3,+8,-13,+6,+3,-5,+6,+5,+1,+12,+14,+10,+12,-20,+12,-15,+8,+4,-7,-9,-1,+11,-22,-4,-14,-4,-18,+6,+2,+8,-4,-1,+16,-17,-1,-2,-10,-10,-10,+16,+3,-12,+2,-1,+4,-15,+18,-2,-9,-6,-22,+7,+16,-5,+17,+13,+2,-1,+3,-11,+10,-25,-14,+17,-15,+3,+43,+19,+12,+1,-8,+4,+8,-11,-40,-29,-30,-12,+18,-15,-5,-14,+10,-16,+1,-34,-13,-20,-7,+12,+9,+16,-2,-2,+9,+23,+15,-8,-38,-37,-8,-20,+11,-24,-2,+14,-24,-18,+2,-7,+14,+1,+19,-5,-23,-11,-15,+23,+8,-14,+16,-25,-19,+8,-3,-20,+11,-15,-11,-14,+16,-31,+18,+25,+20,-6,+11,+48,-7,-18,-2,-31,+21,-49,+50,-20,-44,+8,-81,+24,-16,-18,+26,-60,+19,-36,-14,-41,+25,-72,-46,+1,-46,-61361,+12,-18,-7,-9,+6,+14,-13,-8,+19,+19,+11,+9,+2,+9,-18,-3,+20,-4,-11,+4,+14,+3,-11,+2,+2,-13,-17,+3,+19,-13,-16,+11,+14,-5,+12,+16,+17,+15,+6,+6,+2,-1,+15,+6,-3,-5,-6,+20,+16,+7,+1,-5,+9,+12,-2,+11,-16,+6,-14,-3,-11,-8,-18,-9,+2,+12,-15,+20,+1,+1,+10,+5,+17,+7,-2,-14,+1,+10,-1,+12,+12,-37,-13,-3,+5,+3,+18,-14,-26,+11,-14,-3,-14,-6,-21,+2,-8,-3,+18,-3,+18,-10,-15,-6,+10,+5,+17,+14,-16,-10,+6,-2,-1,-6,-12,+14,-5,-11,-11,+7,-6,+9,+20,-11,-21,+14,-7,-8,-29,-12,-2,-3,-17,+9,-7,+13,+18,-9,-11,+12,+11,-1,-24,-7,-14,-13,-18,+9,-16,+19,+18,+16,+2,-13,-19,+7,-19,-1,-8,-16,+2,+15,-16,+18,+13,-19,-3,-2,-4,-14,+9,+10,-14,-15,+6,+8,-12,-11,-3,-12,-4,-18,+14,-13,-10,-14,+15,+3,-15,+2,+1,-9,-11,-1,-17,+16,-5,-2,+19,-8,-19,-3,-19,+12,-17,+6,+16,+19,+4,-5,-4,-11,+13,+15,+7,-11,+16,+4,-1,+18,-15,+20,-11,+8,-13,+19,+7,+11,+19,-1,-7,-17,-2,-19,-11,+15,-11,-16,+15,-10,+25,-5,+9,+17,-16,+12,+10,+5,-2,-17,+11,+16,-4,-3,-4,-4,-3,-7,+4,-9,-19,+1,-5,+29,+2,+28,+15,+20,-15,-13,+20,+20,+3,-10,-11,+20,+5,+14,+6,-18,-13,+1,+15,+30,-23,+28,-7,-10,-19,-21,+20,-5,+12,-9,-10,-9,-15,-23,-1,+17,-2,-10,-1,-13,+28,+22,-6,-8,+43,+42,-8,+27,+4,-18,+49,+19,-8,+9,-2,-1,+37,-12,-11,+17,+15,-6,+24,+19,+6,-74,+6,+26,-317,+4,-23,-9,-17,-17,+13,-19,+8,+14,-6,-6,+1,+10,+10,-1,+9,-7,+18,-9,-8,-4,+7,+18,-3,-16,+18,-5,+11,+12,-1,+9,+11,-14,-9,+19,+24,+30,-27,+76,-35,-34,-82,+14,-25,-10,-3,-7,-14,+16,-4,-2,-5,-18,+9,-7,-1,+6,-15,+11,+18,-4,+16,-9,+11,-16,-25,-11,+21,+6,+5,-10,+7,+20,+22,-10,+4,-10,-16,-6,-18,-31,-69,-12,-18,-25,-8,+49,+50,-19,+60,-16,-96,+200,+34,-148,-153,-45,-1,-45,-61788,-16,-5,-8,-6,+3,-6,+16,+18,+10,-14,+9,-20,+6,+18,+9,+13,+7,+11,-5,-11,+17,+13,-4,-4,-3,-11,-17,-2,+4,+9,-15,-3,+19,-9,-12,-17,+5,-14,+17,-7,-23,-11,+3,-13,+4,+19,-14,-17,+10,+1,+4,-11,+9,-17,+7,-16,+12,-8,+1,-9,-9,-7,-7,+1,+1,+4,+2,-16,+7,+4,-18,-7,+12,+7,-1,-14,+9,-17,-8,+13,+19,-18,-17,-14,-10,-9,-12,-19,-17,+14,-16,+18,-15,-14,-17,-9,-14,-15,-19,-19,+10,-17,-2,+11,+15,+9,+1,+17,+17,-10,-16,+15,+4,-7,+10,+5,-16,-5,-5,-3,-8,+17,-5,+124726};
    long int frequency = 0;
    cout << "size of input: " << size(puzzInput) << endl;

    unordered_set<long int> seenFreq;
    seenFreq.emplace(frequency);
    bool duplicateFound = false;

    auto iter = puzzInput.begin();
    while (iter != puzzInput.end()) {
        frequency += (*iter);

        auto setIter = seenFreq.find(frequency);
        if (setIter != seenFreq.end()) {
            cout << "already seen: " << (*setIter) << endl;
            break;
            duplicateFound = true;
        } else {
            seenFreq.emplace(frequency);
        }

        iter++;
        
        if (iter == puzzInput.end()) {
            cout << "reverting to the beginning" << endl;
            iter = puzzInput.begin();
        }
    }

    cout << "size: " << seenFreq.size() << endl;

    return 0;
}
