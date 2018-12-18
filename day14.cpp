#include <iostream>
#include <list>
#include <algorithm>
#include <deque>
#include <iterator>
#include <vector>
#include <string>

using namespace std;

int main(int argc, char** argv) {

	//int numRecipesAfter = stoi(argv[1]);
	int numToPrint = 10;
	vector<int> testSeq = {6,3,3,6,0};
	//vector<int> testSeq = {9,2,5,1,0};

	vector<int> recipes;
	//recipes.reserve(numRecipesAfter + numToPrint);
	recipes.push_back(3);
	recipes.push_back(7);
	unsigned int elf1 = 0;
	unsigned int elf2 = 1;
	deque<int> newNums;
	deque<int> lastFive;
	lastFive.push_back(recipes[0]);
	lastFive.push_back(recipes[1]);
	bool seqMatched = false;

	while (seqMatched == false) {
		// get new recipes.
		unsigned int newNum = recipes[elf1] + recipes[elf2];
		//cout << newNum;
		if (newNum == 0) {
			recipes.push_back(0);
			lastFive.push_back(0);
		} else {
			while(newNum) {
				newNums.push_back(newNum % 10);
				newNum /= 10;
			}

			while(newNums.empty() == false) {
				recipes.push_back(newNums.back());
				lastFive.push_back(newNums.back());
				newNums.pop_back();
			}
		}

		// march elf1
		elf1 += (1 + recipes[elf1]);
		elf1 %= recipes.size();

		// march elf2
		elf2 += (1 + recipes[elf2]);
		elf2 %= recipes.size();

		while (lastFive.size() > testSeq.size()) {
			lastFive.pop_front();
		}

		//for (auto it = lastFive.begin(); it != lastFive.end(); it++) { cout << *it << endl;}
		seqMatched = true;
		for (int i = 0; i < lastFive.size(); i++) {
			if (lastFive[i] != testSeq[i]) {
				seqMatched = false;
				break;
			}
		}
	}
	cout << "Num recipes: " << recipes.size() - lastFive.size() << endl;
	
	/*vector<int> testSeq = {6,3,3,6,0,1};
	
	bool found = false;
	for (int i = numRecipesAfter; (i < numRecipesAfter + numToPrint); i++) {
		for (int j = 0; j < size(testSeq); j++) {
			if (recipes[i + j] == testSeq[j]) {
				found = true;
			} else {
				found = false;
				break;
			}
		}
		if (found == true) {
			cout << "Starts at: " << i << endl;
			break;
		}
	}
	cout << endl;
*/
	return 0;
}