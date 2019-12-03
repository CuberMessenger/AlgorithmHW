#include <iostream>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <algorithm>
using namespace std;

//判断纸带上一个4位的二进制数是不是奇数
//奇数 -> accept
//偶数 -> reject
int pointer = 0;
bool M(string paperTape) {
	if (paperTape == "")
		return false;

	//left shift
	pointer++;
	//left shift
	pointer++;
	//left shift
	pointer++;
	//read
	char lowestBit = paperTape[pointer];
	if (lowestBit == '0')
		return false;//reject
	else
		return true;//accept
}

int main() {
	srand(time(0));
	int temp = rand() % 0x00000010;
	string paperTape = "";
	for (int i = 0; i < 4; i++, temp >>= 1) {
		paperTape += temp & 0x00000001 ? "1" : "0";
	}
	cout << paperTape << "\t";
	printf(M(paperTape) ? "True\n" : "False\n");
}