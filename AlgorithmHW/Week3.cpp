#include <iostream>
#include <cstdio>
#include <string>
using namespace std;

//输出所有不同的组合
//a中每个char表示一个元素
void FindAllCombination(string a) {
	string cur;
	unsigned int x = 1;
	for (int i = 1; i < (1 << a.length()); i++, x++) {
		cur = "";
		unsigned int temp = x;
		for (int index = 0; temp; index++, temp >>= 1) {
			if (temp & 0x00000001) {
				cur += a[index];
			}
		}
		printf("%s\n", cur.c_str());
	}
	cout << "Num of Combination: " << x - 1 << "\n\n";
}

//简单地按照递归关系实现整数分解的方式个数
int IntegerDivision(int n, int m) {
	if (m == 1)
		return 1;
	if (n < m)
		return IntegerDivision(n, n);
	if (n == m)
		return 1 + IntegerDivision(n, n - 1);
	if (n > m && m > 1)
		return IntegerDivision(n, m - 1) + IntegerDivision(n - m, m);
}

#define N 1001

int mat[N][N];
//直接递归速度很慢，改写down-top DP提速
int IntegerDivisionDP(int n) {
	if (n >= N)
		return -1;

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			mat[i][j] = -1;
		}
	}

	for (int i = 1; i < N; i++) {
		for (int j = 1; j < N; j++) {
			if (j == 1)
				mat[i][j] = 1;
			else if (i < j)
				mat[i][j] = mat[i][i];
			else if (i == j)
				mat[i][j] = mat[i][i - 1] + 1;
			else if (i > j && j > 1)
				mat[i][j] = mat[i][j - 1] + mat[i - j][j];
		}
	}

	return mat[n][n];
}


int main() {
	string a;
	while (cin >> a, a != "end") {
		FindAllCombination(a);
	}

	int n;
	while (cin >> n, n > 0) {
		//cout << IntegerDivision(n, n) << endl;

		cout << IntegerDivisionDP(n) << endl;
	}
}