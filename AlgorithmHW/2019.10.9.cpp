#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

#define N 5

void OptimalBinarySearchTree(double* a, double* b, int n, double** m, int** s, double** w) {
	for (int i = 1; i <= n + 1; i++) {
		m[i][i - 1] = 0;
		w[i][i - 1] = a[i - 1];
	}

	for (int l = 1; l <= n; l++) {
		for (int i = 1, j; i <= n - l + 1; i++) {
			j = i + l - 1;
			m[i][j] = (0x7FFFFFFF) * 1.0;
			s[i][j] = i;
			w[i][j] = w[i][j - 1] + a[j] + b[j];

			for (int k = i; k <= j; k++) {
				double t = w[i][j] + m[i][k - 1] + m[k + 1][j];
				if (t < m[i][j]) {
					m[i][j] = t;
					s[i][j] = k;
				}
			}
		}
	}
}

string IntegerToString(int n) {
	string ans = "";
	while (n) {
		int t = n % 10;
		ans = string(1, (char)(t + (int)'0')) + ans;
		n /= 10;
	}
	return ans;
}

string ParenthesesTree(int i, int j, int** s) {
	if (j <= i - 1)
		return "";
	if (j == i)
		return "x" + IntegerToString(i);
	int k = s[i][j];
	return "(" + ParenthesesTree(i, k - 1, s) + ")" +
		"x" + IntegerToString(k) +
		"(" + ParenthesesTree(k + 1, j, s) + ")";
}

struct Tree {
	string name;
	int level;
	double p;
	Tree* left;
	Tree* right;
	Tree(string n, int l, double p) {
		this->name = n;
		this->level = l;
		this->p = p;
		this->left = nullptr;
		this->right = nullptr;
	}
};
vector<Tree*> roots;

Tree* BuildTree(int i, int j, int level, int** s, double* a, double* b) {
	if (j <= i - 1)
		return nullptr;

	int k = s[i][j];
	Tree* ans = new Tree("x" + IntegerToString(k), level, b[k]);
	if (j == i) {

		Tree* left = new Tree("d" + IntegerToString(k - 1), level, a[k - 1]);
		Tree* right = new Tree("d" + IntegerToString(k), level, a[k]);
		ans->left = left;
		ans->right = right;
	}
	else {
		//left is virtual?
		if (i == k) {
			Tree* left = new Tree("d" + IntegerToString(k - 1), level, a[k - 1]);
			ans->left = left;
		}
		else {
			ans->left = BuildTree(i, k - 1, level + 1, s, a, b);
		}

		//right is virtual?
		if (j == k) {
			Tree* right = new Tree("d" + IntegerToString(k), level, a[k]);
			ans->right = right;
		}
		else {
			ans->right = BuildTree(k + 1, j, level + 1, s, a, b);
		}
	}
	roots.push_back(ans);
	return ans;
}

double CalculateAverageCost(Tree* root) {
	if (root == nullptr)
		return 0.0;
	if (root->left == nullptr || root->right == nullptr)
		return root->p * root->level;
	else
		return CalculateAverageCost(root->left) + root->p * root->level + CalculateAverageCost(root->right);
}

int main() {
	//vitual key p
	//double a[N + 1] = { 0.10, 0.07, 0.01, 0.06, 0.02, 0.05, 0.03, 0.04, 0.03, 0.04, 0.05 };
	//key p
	//double b[N + 1] = { 0.00, 0.01, 0.09, 0.02, 0.08, 0.06, 0.04, 0.05, 0.03, 0.05, 0.07 };


	double a[N + 1] = { 0.05,0.1,0.05,0.05,0.05,0.1 };

	double b[N + 1] = { 0.0,0.15,0.1,0.05,0.1,0.2 };


	int** s = new int* [N + 1];
	for (int i = 0; i < N + 1; i++) {
		s[i] = new int[N + 1];
		for (int j = 0; j < N + 1; j++) {
			s[i][j] = 0;
		}
	}
	double** m = new double* [N + 2];
	double** w = new double* [N + 2];
	for (int i = 0; i < N + 2; i++) {
		m[i] = new double[N + 2];
		w[i] = new double[N + 2];
	}

	OptimalBinarySearchTree(a, b, N, m, s, w);
	cout << endl << ParenthesesTree(1, N, s) << endl;
	Tree* root = BuildTree(1, N, 1, s, a, b);
	cout << roots.size() << endl;
	cout << "以" << root->name << "为根的子树的平均比较次数为：" << CalculateAverageCost(root) << endl;

	for (int i = 0; i < N; i++) {
		cout << "以" << roots[i]->name << "为根的子树的平均比较次数为：" << CalculateAverageCost(roots[i]) << endl;
	}
}