#include <iostream>
#include <cstdio>
#include <string>
#include <algorithm>
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

//////////////////////////////////////////////////////////

void PrintDivision(string s, int n, int m) {
	if (n <= 0) {
		//如下组合的字符串形如+1+1+...+2+2+...
		//调整为从大到小，并去掉多余加号
		s.erase(s.begin());
		reverse(s.begin(), s.end());
		cout << s << endl;
		return;
	}

	//遍历最大因子1~n，当最大因子为m时，在PD(n-m, m)的解（可能有若干个，所以用递归比较方便）上再加一个m就是PD(n, m)的解
	for (int i = m; i <= n; i++)
		PrintDivision(s + "+" + (char)(i + '0'), n - i, i);
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

		//cout << IntegerDivisionDP(n) << endl;
		PrintDivision("", n, 1);
	}
}