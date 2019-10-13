#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

int LCS(string x, string y, int** c, int** s) {
	int m = x.size();
	int n = y.size();
	for (int i = 1; i <= m; i++) {
		for (int j = 1; j <= n; j++) {
			if (x[i - 1] == y[j - 1]) {
				c[i][j] = c[i - 1][j - 1] + 1;
				s[i][j] = 1;
			}
			else if (c[i - 1][j] >= c[i][j - 1]) {
				c[i][j] = c[i - 1][j];
				s[i][j] = 2;
			}
			else {
				c[i][j] = c[i][j - 1];
				s[i][j] = 3;
			}
		}
	}
	return c[m][n];
}

void PrintLCS(string x, int** s, int i, int j) {
	if (i == 0 || j == 0) {
		return;
	}
	if (s[i][j] == 1) {
		PrintLCS(x, s, i - 1, j - 1);
		cout << x[i - 1];
	}
	else if (s[i][j] == 2) {
		PrintLCS(x, s, i - 1, j);
	}
	else {
		PrintLCS(x, s, i, j - 1);
	}
}

void PrintLCSWithoutS(string x, string y, int** c, int i, int j) {
	if (i == 0 || j == 0) {
		return;
	}
	if (x[i - 1] == y[j - 1]) {
		PrintLCSWithoutS(x, y, c, i - 1, j - 1);
		cout << x[i - 1];
	}
	else if (c[i - 1][j] >= c[i][j - 1]) {
		PrintLCSWithoutS(x, y, c, i - 1, j);
	}
	else {
		PrintLCSWithoutS(x, y, c, i, j - 1);
	}
}

int main_2019_9_30() {
	string x, y;
	while (cin >> x >> y, x != "end") {
		int m = x.size();
		int n = y.size();

		int** c = new int* [m + 1];
		int** s = new int* [m + 1];
		for (int i = 0; i < m + 1; i++) {
			c[i] = new int[n + 1];
			s[i] = new int[n + 1];
			for (int j = 0; j < n + 1; j++) {
				c[i][j] = 0;
			}
		}

		cout << LCS(x, y, c, s) << endl;
		//PrintLCS(x, s, m, n);
		cout << endl;
		PrintLCSWithoutS(x, y, c, m, n);
	}
	return 0;
}

/*
abcabcdeabcdefg
abdcabcdeabgcdefg

13456778
357486782

*/