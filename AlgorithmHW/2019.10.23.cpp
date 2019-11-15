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
					goto NextT;
			}
			for (int j = 1; j <= t; j++) {//<-^
				if (t - j >= 0 && x[t - j] == x[t] - j)
					goto NextT;
			}
			for (int j = 1; j <= t; j++) {//^->
				if (t - j >= 0 && x[t - j] == x[t] + j)
					goto NextT;
			}

			if (NQueenBacktraceRecursive(t + 1, n, x))
				return true;

		NextT:
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
					goto NextI;
			}
			for (int j = 1; j <= t; j++) {//<-^
				if (t - j >= 0 && x[t - j] == x[t] - j)
					goto NextI;
			}
			for (int j = 1; j <= t; j++) {//^->
				if (t - j >= 0 && x[t - j] == x[t] + j)
					goto NextI;
			}
			goto NextT;

		NextI:
			continue;
		}
		x[t] = -1;
		is[t] = 0;
		t--;
		is[t]++;
		continue;

	NextT:
		t++;
	}
	PrintQueen(n, x);
}

//////////////////////////////////////////////////////////

int Perform(int x, int y, int op) {
	if (op == 0)
		return x + y;
	if (op == 1)
		return x - y;
	if (op == 2)
		return x * y;
	if (op == 3) {
		if (y != 0 && x % y == 0)
			return x / y;
		else
			return -0x0000FFFF;
	}
}

int Evaluate(vector<int>& ansN, vector<int>& ansS, bool split) {
	if (split) {
		int ansl = Perform(ansN[0], ansN[1], ansS[0]);
		int ansr = Perform(ansN[2], ansN[3], ansS[2]);
		return Perform(ansl, ansr, ansS[1]);
	}
	else {
		int ans1 = Perform(ansN[0], ansN[1], ansS[0]);
		int ans2 = Perform(ans1, ansN[2], ansS[1]);
		return Perform(ans2, ansN[3], ansS[2]);
	}
}

const char OperatorDictionary[4] = { '+', '-', '*', '/' };

void PrintAnswer(vector<int>& ansN, vector<int>& ansS, bool split) {
	if (split) {
		int ansl = Perform(ansN[0], ansN[1], ansS[0]);
		printf("%d %c %d = %d\n", ansN[0], OperatorDictionary[ansS[0]], ansN[1], ansl);
		int ansr = Perform(ansN[2], ansN[3], ansS[2]);
		printf("%d %c %d = %d\n", ansN[2], OperatorDictionary[ansS[2]], ansN[3], ansr);
		int ans = Perform(ansl, ansr, ansS[1]);
		printf("%d %c %d = %d\n", ansl, OperatorDictionary[ansS[1]], ansr, ans);
	}
	else {
		int ans1 = Perform(ansN[0], ansN[1], ansS[0]);
		printf("%d %c %d = %d\n", ansN[0], OperatorDictionary[ansS[0]], ansN[1], ans1);
		int ans2 = Perform(ans1, ansN[2], ansS[1]);
		printf("%d %c %d = %d\n", ans1, OperatorDictionary[ansS[1]], ansN[2], ans2);
		int ans = Perform(ans2, ansN[3], ansS[2]);
		printf("%d %c %d = %d\n", ans2, OperatorDictionary[ansS[2]], ansN[3], ans);
	}
}

bool Make24Points(int a, int b, int c, int d) {
	vector<int> ansN = { a, b, c, d };
	vector<int> ansS = { 0, 0, 0 };

NextPermutation:
	for (int op1 = 0; op1 < 4; op1++) {
		for (int op2 = 0; op2 < 4; op2++) {
			for (int op3 = 0; op3 < 4; op3++) {
				ansS[0] = op1;
				ansS[1] = op2;
				ansS[2] = op3;
				if (Evaluate(ansN, ansS, false) == 24) {
					PrintAnswer(ansN, ansS, false);
					return true;
				}
				if (Evaluate(ansN, ansS, true) == 24) {
					PrintAnswer(ansN, ansS, true);
					return true;
				}
			}
		}
	}

	if (next_permutation(ansN.begin(), ansN.end()))
		goto NextPermutation;
	return false;
}

int main_2019_10_23() {
	int n;
	while (cin >> n, n > 0) {

		int* x = new int[n];
		for (int i = 0; i < n; i++) {
			x[i] = -1;
		}
		//NQueenBacktraceRecursive(0, n, x);
		NQueenBacktraceIterative(n, x);
	}

	int a, b, c, d;
	while (cin >> a >> b >> c >> d, a > 0 && b > 0 && c > 0 && d > 0) {
		cout << (Make24Points(a, b, c, d) ? "Solved" : "Unsolvable") << endl << endl;
	}
	return 0;
}