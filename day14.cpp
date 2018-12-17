#include <iostream>
#include <list>
#include <algorithm>
#include <deque>
#include <iterator>

using namespace std;

int main(int argc, char** argv) {

	int numRecipesAfter = stoi(argv[1]);
	int numToPrint = 10;

	list<int> recipes;
	recipes.push_back(3);
	recipes.push_back(7);
	auto elf1 = recipes.begin();
	auto elf2 = recipes.begin();
	elf2++;
	deque<int> newNums;
	cout << "Max list size: " << recipes.max_size() << endl;

	while (recipes.size() <= (numRecipesAfter + numToPrint)) {
		int i = *elf1 + 1;
		int j = *elf2 + 1;

		// get new recipes.
		int newNum = *elf1 + *elf2;
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
			if (elf1 == recipes.end()) {
				elf1 = recipes.begin();
			}		
		}

		// march elf2
		while (j-- > 0) {
			elf2++;
			if (elf2 == recipes.end()) {
				elf2 = recipes.begin();
			}		
		}

		if (elf1 == elf2) {			
			cout << "same" << endl;
			if (elf2 == recipes.end()) {
				elf2 = recipes.begin();
			}
			elf2++;
		}
		//cout << "\r" << recipes.size();
	}
	cout << endl;
	int numRcpsAfter = numRecipesAfter;
	auto it = recipes.begin();
	while(numRcpsAfter) {
		numRcpsAfter--;
		it++;
	}


	while(numToPrint) {
		cout << *it;
		it++;
		numToPrint--;
	}

	return 0;
}