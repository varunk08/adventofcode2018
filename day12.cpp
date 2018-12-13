#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <bitset>
#include <algorithm>

using namespace std;
const int BitSetWidth = 200;
const int Shift = 0;//3;// (BitSetWidth - 100) / 2;

int main (int argc, char** argv) {
    string input = "..#..####.##.####...#....#######..#.#..#..#.#.#####.######..#.#.#.#..##.###.#....####.#.#....#.#####";
    reverse(input.begin(), input.end());
    cout << "input: " << input << endl;

    std::bitset<BitSetWidth> plants(input, 0, input.size(), '.', '#');  
    plants <<= Shift;
    for (int i = 0; i < plants.size(); i++) {
        if (plants[i] == 1) { cout << "#";}
        else { cout << ".";}
    }
    cout << endl;

    unordered_map<std::bitset<5>, bool> rules;
    vector<vector<string>> inputRules = {
{ "#.##.", "." },
{ "#.#..", "." },
{ "###.#", "." },
{ "..#.#", "." },
{ "....#", "." },
{ ".####", "." },
{ "##.##", "#" },
{ "###..", "#" },
{ ".###.", "#" },
{ "...#.", "." },
{ ".....", "." },
{ "##..#", "." },
{ ".#.#.", "#" },
{ ".#.##", "#" },
{ "##.#.", "." },
{ "##...", "." },
{ "#####", "#" },
{ "#...#", "." },
{ "..##.", "." },
{ "..###", "." },
{ ".#...", "#" },
{ ".##.#", "." },
{ "#....", "." },
{ ".#..#", "." },
{ ".##..", "#" },
{ "...##", "#" },
{ "#.###", "." },
{ "#..#.", "." },
{ "..#..", "#" },
{ "#.#.#", "#" },
{ "####.", "#" },
{ "#..##", "." },
    };

    for (auto vec : inputRules) {
        string ruleStr = vec[0];
        reverse(ruleStr.begin(), ruleStr.end());
        bitset<5> rule (ruleStr, 0, ruleStr.size(), '.', '#');
        bool enabled = (vec[1] == ".") ? false : true;

        rules.emplace(rule, enabled);
    }

    long long int totalShift = Shift;
    long long unsigned int generation = 0;

    while (generation < 1000) {
        unsigned int leftMostSet = 0;
        bitset<BitSetWidth> nextGen;

        for (int i = 0; i < size(plants); i++) {
            bitset<5> curBitset;

            curBitset[0] = (i >= 2) ? plants[i - 2] : 0;
            curBitset[1] = (i >= 1) ? plants[ i - 1] : 0;
            curBitset[2] = plants[i]; // center pot
            curBitset[3] = (i < size(plants) - 1) ? plants[i + 1] : 0;
            curBitset[4] = (i < size(plants) - 2) ? plants[i + 2] : 0;

            auto it = rules.find(curBitset);
            if (it != rules.end()) {
                nextGen[i] = (it->second == false) ? 0 : 1;
            } else {
                cout << "rule not found " << curBitset.to_string() << endl;
            }
        }
        cout << generation + 1 << ": ";
        for (int i = 0; i < nextGen.size(); i++) {
            if (nextGen[i] == 1) { cout << "#";}
            else { cout << ".";}
        }
        cout << endl;
        plants = nextGen;

        // find left most bit set
        for (int i = 0; i < plants.size(); ++i) {
            if (plants[i] == 1) {
                leftMostSet = i;
                break;
            }
        }

        if (leftMostSet >= 2) {
            plants >>= (leftMostSet - 2);
            totalShift -= (leftMostSet - 2);
        } else {
            plants <<= (2 - leftMostSet);
            totalShift += (2 - leftMostSet);
        }
        cout << "shift: " << totalShift << " " << leftMostSet << "    ";
        generation++;
    }

    long long int finalShift = -49999999903;
    long long int score1 = 0;
    for (int i = 0; i < plants.size(); i++) {
        if (plants[i] == 1) {
            score1 += (i - finalShift);
            long long int idx = (i - finalShift);
            cout << idx << " ";
        }
    }
    cout << "score: " << score1 << endl;
    
    return 0;
}