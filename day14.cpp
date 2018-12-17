#include <iostream>
#include <list>
#include <algorithm>
#include <deque>
#include <iterator>
#include <vector>

using namespace std;

int main(int argc, char** argv) {

	int numRecipesAfter = stoi(argv[1]);
	int numToPrint = 10;

	vector<int> recipes;
	recipes.push_back(3);
	recipes.push_back(7);
	unsigned int elf1 = 0;
	unsigned int elf2 = 1;
	deque<int> newNums;

	while (recipes.size() <= (numRecipesAfter + numToPrint)) {
		int i = recipes[elf1] + 1;
		int j = recipes[elf2] + 1;

		// get new recipes.
		int newNum = recipes[elf1] + recipes[elf2];
		while(newNum) {
			newNums.push_back(newNum % 10);
			newNum /= 10;
		}

		while(newNums.empty() == false) {
			recipes.push_back(newNums.back());
			newNums.pop_back();
		}

		// march elf1
		while (i-- > 0) {
			elf1++;
			if (elf1 == recipes.size()) {
				elf1 = 0;
			}		
		}

		// march elf2
		while (j-- > 0) {
			elf2++;
			if (elf2 == recipes.size()) {
				elf2 = 0;
			}		
		}

		if (elf1 == elf2) {
			cout << "same" <<endl;
		}
	}
	cout << endl;

	auto it = recipes.begin();
	while(numRecipesAfter) {
		//cout << *it;
		numRecipesAfter--;
		it++;
	}

	while(numToPrint) {
		cout << *it;
		it++;
		numToPrint--;
	}

	return 0;
}