#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

void PrintQueen(int n, int* x) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++)
			cout << ((x[i] == j) ? "X" : "O");
		cout << endl;
	}
}

bool NQueenBacktraceRecursive(int t, int n, int* x) {
	if (t == n) {
		PrintQueen(n, x);
		return true;
	}
	else {
		for (int i = 0; i < n; i++) {
			x[t] = i;

			for (int j = 0; j < t; j++) {
				if (j != t && x[j] == x[t])
					goto toNext;
			}
			for (int j = 1; j <= t; j++) {//<-^
				if (t - j >= 0 && x[t - j] == x[t] - j)
					goto toNext;
			}
			for (int j = 1; j <= t; j++) {//^->
				if (t - j >= 0 && x[t - j] == x[t] + j)
					goto toNext;
			}

			if (NQueenBacktraceRecursive(t + 1, n, x))
				return true;

		toNext:
			continue;
		}
	}
	return false;
}

void NQueenBacktraceIterative(int n, int* x) {
	int* is = new int[n];
	for (int i = 0; i < n; i++) {
		is[i] = 0;
	}
	for (int t = 0; t < n;) {
		for (; is[t] < n; is[t]++) {
			x[t] = is[t];

			for (int j = 0; j < t; j++) {
				if (j != t && x[j] == x[t])
					goto nextI;
			}
			for (int j = 1; j <= t; j++) {//<-^
				if (t - j >= 0 && x[t - j] == x[t] - j)
					goto nextI;
			}
			for (int j = 1; j <= t; j++) {//^->
				if (t - j >= 0 && x[t - j] == x[t] + j)
					goto nextI;
			}
			goto nextT;

		nextI:
			continue;
		}
		x[t] = -1;
		is[t] = 0;
		t--;
		is[t]++;
		continue;

	nextT:
		t++;
	}
	PrintQueen(n, x);
}

int main() {
	int n;
	while (cin >> n, n > 0) {

		int* x = new int[n];
		for (int i = 0; i < n; i++) {
			x[i] = -1;
		}
		//NQueenBacktraceRecursive(0, n, x);
		NQueenBacktraceIterative(n, x);
	}
	return 0;
}