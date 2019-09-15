#include <iostream>
#include <cstdio>
#include <string>
#include <algorithm>
using namespace std;

//������в�ͬ�����
//a��ÿ��char��ʾһ��Ԫ��
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
		//������ϵ��ַ�������+1+1+...+2+2+...
		//����Ϊ�Ӵ�С����ȥ������Ӻ�
		s.erase(s.begin());
		reverse(s.begin(), s.end());
		cout << s << endl;
		return;
	}

	//�����������1~n�����������Ϊmʱ����PD(n-m, m)�Ľ⣨���������ɸ��������õݹ�ȽϷ��㣩���ټ�һ��m����PD(n, m)�Ľ�
	for (int i = m; i <= n; i++)
		PrintDivision(s + "+" + (char)(i + '0'), n - i, i);
}

#define N 1001

int mat[N][N];
//ֱ�ӵݹ��ٶȺ�������дdown-top DP����
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