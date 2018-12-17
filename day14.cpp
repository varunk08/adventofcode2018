#include <iostream>
#include <list>
#include <algorithm>
#include <deque>

using namespace std;

int main(int argc, char** argv) {

	const int numRecipesAfter = 18;
	int n = numRecipesAfter;
	n-=2; // we have 3,7 already
	n-=1; // 0 based indices
	n+= 10;

	list<int> recipes;
	recipes.push_back(3);
	recipes.push_back(7);
	auto elf1 = recipes.begin();
	auto elf2 = recipes.begin();
	elf2++;
	deque<int> newNums;

	while (n > 0) {
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
		while (i > 0) {
			elf1++;
			if (elf1 == recipes.end()) {
				elf1 = recipes.begin();
			}
			i--;		
		}

		// march elf2
		while (j > 0) {
			elf2++;
			if (elf2 == recipes.end()) {
				elf2 = recipes.begin();
			}
			j--;		
		}

		n--;
	}

	int numRcpsAfter = numRecipesAfter;
	auto it = recipes.begin();
	while(numRcpsAfter && (it != recipes.end())) {
		numRcpsAfter--;
		it++;
	}

	int numToPrint = 10;
	while(numToPrint && (it != recipes.end())) {
		cout << *it;
		it++;
		numToPrint--;
	}

	return 0;
}