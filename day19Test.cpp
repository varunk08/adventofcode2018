#include <iostream>

using namespace std;

int main(int argc, char** argv) {
	
	int C = 1;
	int F = 1;
	int E = 10551326;
	long long int A = 0;

	for (int i = 1; i <= E; i++) {
		if (E % i == 0) {
			A += i;
		}
	}

	cout << "A: " << A << endl;
	return 0;
}