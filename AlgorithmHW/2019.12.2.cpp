#include <iostream>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <algorithm>
using namespace std;

class TuringMachine {
private:
	string PaperTape;
	int Pointer;
public:
	TuringMachine() {
		Pointer = 0;
	}

	void GivePaperTape(string paperTape) {
		this->PaperTape = paperTape;
	}

	void Left() {
		if (Pointer > 0) {
			Pointer--;
		}
	}

	void Right() {
		if (Pointer < PaperTape.length()) {
			Pointer++;
		}
	}

	char Get() {
		return PaperTape[Pointer];
	}

	bool Judge(bool L(TuringMachine*)) {
		return L(this);
	}
};

//判断纸带上一个4位的二进制数是不是奇数
//奇数 -> accept
//偶数 -> reject
bool TuringIsOdd(TuringMachine* M) {
	M->Right();
	M->Right();
	M->Right();
	if (M->Get() == '1') {
		return true;
	}
	else {
		return false;
	}
}

int main() {
	srand(time(0));
	for (int x = 0; x < 10; x++) {
		int temp = rand() % 0x00000010;
		string paperTape = "";
		for (int i = 0; i < 4; i++, temp >>= 1) {
			paperTape += temp & 0x00000001 ? "1" : "0";
		}
		cout << paperTape << "\t";

		TuringMachine turingMachine;
		turingMachine.GivePaperTape(paperTape);

		printf(turingMachine.Judge(*TuringIsOdd) ? "True\n" : "False\n");
	}
}