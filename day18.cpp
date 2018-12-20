#include <algorithm>
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

int AdjAreaTreeCount(vector<vector<char>>& grid, int x, int y) {
	int treeCount = 0;
	for (int i = x - 1; i <= x + 1; i++) {
		for (int j = y - 1; j <= y + 1; j++) {
			if (i == x && j == y) {
				continue;
			}
			
			if (i >= 0 && i < grid[0].size() && j >= 0 && j < grid.size()) {
				if (grid[i][j] == '|') {
					treeCount++;
				}
			}
		}
	}
	return treeCount;
}

int AdjAreaLyCount(vector<vector<char>>& grid, int x, int y) {
	int lyCount = 0;
	for (int i = x - 1; i <= x + 1; i++) {
		for (int j = y - 1; j <= y + 1; j++) {
			if (i == x && j == y) {
				continue;
			}
			
			if (i >= 0 && i < grid[0].size() && j >= 0 && j < grid.size()) {
				if (grid[i][j] == '#') {
					lyCount++;
				}
			}
		}
	}
	return lyCount;
}

int main(int argc, char** argv) {
	vector<vector<char>> grid; 
	ifstream inFile("day18Input.txt");
	if (inFile.is_open()) {
		string line;
		while(getline(inFile, line)) {
			vector<char> curLine;
			for (auto ch : line) {
				switch(ch) {
					case '.':
						curLine.push_back('.');
						break;
					case '|':
						curLine.push_back('|');
						break;
					case '#':
						curLine.push_back('#');
						break;		
				}
			}
			grid.push_back(curLine);
		}
	}

	long int numMin = 1000;
	vector<vector<char>> temp(grid.size(), vector<char>(grid[0].size(), '.'));
	for (int x = 0; x < grid.size(); x++) {
		for (int y = 0; y < grid[0].size(); y++) {
			cout << grid[x][y];
			temp[x][y] = grid[x][y];
		}
		cout << endl;
	}
	vector<vector<char>>& dst = grid;
	vector<vector<char>>& src = temp;
	cout << endl;

	while (numMin > 0) {
		//cout << "\r" << numMin;
		for (int x = 0; x < grid.size(); x++) {
			for (int y = 0; y < grid[0].size(); y++) {
				switch(src[x][y]) {
					case '.': {
						if (AdjAreaTreeCount(src, x, y) >= 3) {
							dst[x][y] = '|';
						}
						break;
					}
					case '|': {
						if (AdjAreaLyCount(src, x, y) >= 3) {
							dst[x][y] = '#';
						}
						break;
					}
					case '#' : {
						if (AdjAreaTreeCount(src, x, y) < 1 || AdjAreaLyCount(src, x, y) < 1) {
							dst[x][y] = '.';
						}
						break;
					}
				}
			}			
		}

		// swap aliases.
		vector<vector<char>>& tmp = src;
		src = dst;
		dst = tmp;
		
		cout << endl;
		for (int x = 0; x < grid.size(); x++) {
			for (int y = 0; y < grid[0].size(); y++) {
				cout << grid[x][y];
			}
			cout << endl;
		}
		cout << endl;

		numMin--;
	}

	
	int lumberYard = 0;
	int trees = 0;
	for (int x = 0; x < grid.size(); x++) {
		for (int y = 0; y < grid[0].size(); y++) {
			cout << grid[x][y];
			lumberYard += grid[x][y] == '#' ? 1 : 0;
			trees += grid[x][y] == '|' ? 1 : 0;
		}
		cout << endl;
	}

	cout << "Lumber value: " << trees * lumberYard << endl;

	return 0;
}