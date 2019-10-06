#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>
#include <thread>
using namespace std;

int GetNumberOfCase(int n) {
	if (n <= 2) {
		return 1;
	}
	int ans = 0;
	for (int k = 1; k < n; k++) {
		ans += GetNumberOfCase(k) * GetNumberOfCase(n - k);
	}
	return ans;
}

long long* T;

long long GetNumberOfCaseIterative(int n) {
	if (n <= 2) {
		return 1;
	}
	if (T[n] != -1) {
		return T[n];
	}
	T[n] = 0;
	for (int k = 1; k < n; k++) {
		if (T[k] == -1) {
			T[k] = GetNumberOfCaseIterative(k);
		}
		if (T[n - k] == -1) {
			T[n - k] = GetNumberOfCaseIterative(n - k);
		}
		T[n] += T[k] * T[n - k];
	}
	return T[n];
}

vector<string> GetAllPossibleParentheses(int i, int j) {
	int l = j - i + 1;
	vector<string> ans;

	//ÖÕÖ¹Ìõ¼þ
	if (l == 1) {
		ans.push_back(string(1, (char)('A' + i - 1)));
		return ans;
	}

	for (int k = i; k < j; k++) {
		vector<string> left = GetAllPossibleParentheses(i, k);
		vector<string> right = GetAllPossibleParentheses(k + 1, j);

		for (vector<string>::iterator it = left.begin(); it != left.end(); it++)
			for (vector<string>::iterator jt = right.begin(); jt != right.end(); jt++)
				ans.push_back(((*it).length() == 1 ? *it : "(" + *it + ")")+((*jt).length() == 1 ? *jt : "(" + *jt + ")"));
	}

	return ans;
}

int week5_main() {
	int n;
	while (cin >> n, n != 0) {
		//cout << GetNumberOfCase(n) << endl << endl;

		T = new long long[n + 1];
		for (int i = 0; i < n + 1; i++) {
			T[i] = -1;
		}
		cout << GetNumberOfCaseIterative(n) << endl << endl;
	}

	while (cin >> n, n != 0) {
		vector<string> ans = GetAllPossibleParentheses(1, n);
		for (vector<string>::iterator it = ans.begin(); it != ans.end(); it++) {
			cout << *it << endl;
		}
		cout << endl;
	}
	return 0;
}