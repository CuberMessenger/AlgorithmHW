#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

class BigInteger {
public:
	//Num[0]->���λ
	vector<int> Num;

	//true->+, flase->-
	bool Sign;

	BigInteger() {}

	BigInteger(vector<int> num, bool sign) {
		Num = num;
		Sign = sign;
		for (int i = this->Num.size() - 1; i > 0; i--) {
			if (this->Num[i] == 0) {
				this->Num.erase((this->Num.end()) - 1);
			}
			else {
				break;
			}
		}
	}

	BigInteger(int n, bool sign) {
		if (n == 0) {
			Num.push_back(0);
		}
		else {
			while (n > 0) {
				Num.push_back(n % 10);
				n /= 10;
			}
		}
		Sign = sign;
	}

	void Print() {
		printf(this->Sign ? "" : "-");
		for (int i = this->Num.size() - 1; i >= 0; i--) {
			printf("%d", this->Num[i]);
		}
	}

	BigInteger RightShift(int l) {
		while (l--) {
			this->Num.insert(this->Num.begin(), 0);
		}
		for (int i = this->Num.size() - 1; i > 0; i--) {
			if (this->Num[i] == 0) {
				this->Num.erase((this->Num.end()) - 1);
			}
			else {
				break;
			}
		}
		return *this;
	}

};

int ToInt(BigInteger x) {
	int ans = 0;
	for (int i = x.Num.size() - 1; i >= 0; i--) {
		ans += x.Num[i];
		ans *= 10;
	}
	return ans / 10;
}

//is |x| >= |y|?
bool UnsignedLarger(BigInteger* x, BigInteger* y) {
	if (x->Num.size() > y->Num.size()) {
		return true;
	}
	if (x->Num.size() == y->Num.size()) {
		for (int i = x->Num.size() - 1; i >= 0; i--) {
			if (x->Num[i] > y->Num[i]) {
				return true;
			}
			if (x->Num[i] == y->Num[i] && i == 0) {
				return true;
			}
			if (x->Num[i] == y->Num[i]) {
				continue;
			}
			if (x->Num[i] < y->Num[i]) {
				return false;
			}
		}
	}
	return false;
}

BigInteger SignedAdd(BigInteger x, BigInteger y) {
	vector<int> ans;
	BigInteger shortI = x;
	BigInteger longI = y;

	if (UnsignedLarger(&x, &y)) {
		longI = x;
		shortI = y;
	}

	if (shortI.Sign == longI.Sign) {
		int i = 0, cur, c = 0;
		for (; i < shortI.Num.size(); i++) {
			cur = shortI.Num[i] + longI.Num[i] + c;
			c = cur / 10;
			ans.push_back(cur - c * 10);
		}

		while (i < longI.Num.size()) {
			cur = longI.Num[i++] + c;
			c = cur / 10;
			ans.push_back(cur - c * 10);
		}

		if (c > 0) {
			ans.push_back(c);
		}

		return BigInteger(ans, shortI.Sign);
	}
	else {
		//calc |long| - |short|
		int i = 0, cur, c = 0;
		for (; i < shortI.Num.size(); i++) {
			cur = longI.Num[i] - shortI.Num[i] - c;
			c = 0;
			while (cur < 0) {
				cur += 10;
				c++;
			}
			ans.push_back(cur);
		}

		while (i < longI.Num.size()) {
			cur = longI.Num[i++] - c;
			c = 0;
			while (cur < 0) {
				cur += 10;
				c++;
			}
			ans.push_back(cur);
		}

		if (longI.Sign) {
			return BigInteger(ans, true);
		}
		else {
			return BigInteger(ans, false);
		}
	}
}


BigInteger SignedFastMultiply(BigInteger x, BigInteger y) {
	//��ֹ����
	if (x.Num.size() < 3 && y.Num.size() < 3) {
		return BigInteger(ToInt(x) * ToInt(y), ~(x.Sign ^ y.Sign));
	}

	bool ansSign = ~(x.Sign ^ y.Sign);
	x.Sign = true;
	y.Sign = true;

	BigInteger a, b, c, d;
	int l = y.Num.size() / 2;
	if (x.Num.size() > y.Num.size()) {
		l = x.Num.size() / 2;
	}

	//split
	if (x.Num.size() <= l) {
		a = BigInteger(0, true);
		b = x;
	}
	else {
		vector<int> an, bn;
		for (int i = 0; i < x.Num.size(); i++) {
			if (i < l) {
				bn.push_back(x.Num[i]);
			}
			else {
				an.push_back(x.Num[i]);
			}
		}

		a = BigInteger(an, true);
		b = BigInteger(bn, true);
	}

	if (y.Num.size() <= l) {
		c = BigInteger(0, true);
		d = y;
	}
	else {
		vector<int> cn, dn;
		for (int i = 0; i < y.Num.size(); i++) {
			if (i < l) {
				dn.push_back(y.Num[i]);
			}
			else {
				cn.push_back(y.Num[i]);
			}
		}

		c = BigInteger(cn, true);
		d = BigInteger(dn, true);
	}

	BigInteger ac = SignedFastMultiply(a, c);
	BigInteger bd = SignedFastMultiply(b, d);

	BigInteger a_b_multi_c_d = SignedFastMultiply(SignedAdd(a, b), SignedAdd(c, d));
	ac.Sign = false;
	bd.Sign = false;
	BigInteger p2 = SignedAdd(a_b_multi_c_d, SignedAdd(ac, bd)).RightShift(l);
	ac.Sign = true;
	bd.Sign = true;
	BigInteger p1 = ac.RightShift(2 * l);
	BigInteger f = SignedAdd(p1, SignedAdd(bd, p2));
	f.Sign = ansSign;
	return f;
}

////////////////////////////////////////////////////////////////
void Swap(int& a, int& b) {
	int temp = a;
	a = b;
	b = temp;
}

int Partition5(int list[], int left, int right) {
	int i = left + 1;
	while (i <= right) {
		int j = i;
		while (j > left && list[j - 1] > list[j]) {
			Swap(list[j - 1], list[j]);
			j--;
		}
		i++;
	}
	return (left + right) / 2;
}

int Partition(int list[], int left, int right, int pivotIndex, int n) {
	int val = list[pivotIndex];
	Swap(list[pivotIndex], list[right]);
	int index = left;
	for (int i = left; i < right; i++)
		if (list[i] < val)
			Swap(list[index++], list[i]);

	int indexEq = index;
	for (int i = index; i < right; i++)
		if (list[i] == val)
			Swap(list[indexEq++], list[i]);

	Swap(list[right], list[indexEq]);
	if (n < index)
		return index;
	if (n <= indexEq)
		return n;
	return indexEq;
}

int Pivot(int list[], int left, int right);

int Select(int list[], int left, int right, int n) {
	while (true) {
		if (left == right)
			return left;
		int index = Pivot(list, left, right);
		index = Partition(list, left, right, index, n);
		if (n == index)
			return n;
		else if (n < index)
			right = index - 1;
		else
			left = index + 1;
	}
}

int Pivot(int list[], int left, int right) {
	if (right - left < 5)
		return Partition5(list, left, right);
	for (int i = left; i < right; i += 5) {
		int subRight = (i + 4) > right ? right : i + 4;
		int median = Partition5(list, i, subRight);
		Swap(list[median], list[left + ((i - left) / 5)]);
	}
	int mid = ((right - left) / 10) + left + 1;
	return Select(list, left, left + ((right - left) / 5), mid);
}

int week4_main() {
	string a, b;
	while (cin >> a >> b, a != "end") {
		vector<int> an, bn;
		for (int i = a.size() - 1; i >= 0; i--) {
			an.push_back((int)(a[i] - '0'));
		}
		for (int i = b.size() - 1; i >= 0; i--) {
			bn.push_back((int)(b[i] - '0'));
		}

		BigInteger ans = SignedFastMultiply(BigInteger(an, true), BigInteger(bn, true));
		ans.Print();
		printf("\n\n");
	}

	int arr[] = { 8, 6, 1 ,2 ,9 ,4 ,3 ,7 ,5 };
	int n;
	for (int i = 0; i < 9; i++)
		cout << arr[i] << ' ';
	cout << endl;
	while (cin >> n, n != 0) {
		cout << "The " << n << "th smallest number in the array is: " << Select(arr, 0, 9, n) << endl;
	}

	return 0;
}