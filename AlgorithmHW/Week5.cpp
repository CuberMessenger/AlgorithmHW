#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>
#include <thread>
using namespace std;

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

		for (vector<string>::iterator it = left.begin(); it != left.end(); it++) {
			for (vector<string>::iterator jt = right.begin(); jt != right.end(); jt++) {
				ans.push_back("(" + *it + ")(" + *jt + ")");
			}
		}
	}

	return ans;
}

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

int main() {
	int n;
	while (cin >> n, n != 0) {
		vector<string> ans = GetAllPossibleParentheses(1, n);
		for (vector<string>::iterator it = ans.begin(); it != ans.end(); it++) {
			for (int i = 0; i < (*it).size(); i++) {
				if ((*it)[i] != '(' && (*it)[i] != ')') {
					if (i == 0 || i == (*it).size() - 1) {
						continue;
					}
					if ((*it)[i - 1] == '(' && (*it)[i + 1] == ')') {
						(*it).erase(i - 1, 1);
						i--;
						(*it).erase(i + 1, 1);
					}
				}
			}
		}

		for (vector<string>::iterator it = ans.begin(); it != ans.end(); it++) {
			cout << *it << endl;
		}
		cout << endl;
	}

	while (cin >> n, n != 0) {
		cout << GetNumberOfCase(n) << endl << endl;
	}
}