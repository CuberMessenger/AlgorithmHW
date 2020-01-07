#include <iostream>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <algorithm>
using namespace std;

#define N 500
#define MaxCode 0x00002710 //10000
#define MaxIteration  0x00000FFF

double EvaluateFunction(unsigned int xCode) {
	double x = xCode * 0.0001;
	double answer = x * sin(5 * x);
	return answer < 0.0 ? 0.0 : answer;
}

unsigned int GenerateOffspring(unsigned int parent1, unsigned int parent2) {
	unsigned int answer = 0x0000FFFF;
	while (answer > MaxCode) {
		answer = 0x00000000;
		int position = rand() % 14;
		for (int i = 0; i < position; i++) {
			answer |= parent1 & 0x00000001;
			answer <<= 1;
			parent1 >>= 1;
		}
		parent2 >>= position;
		for (int i = position; i < 14; i++) {
			answer |= parent2 & 0x00000001;
			answer <<= 1;
			parent2 >>= 1;
		}
	}
	return answer;
}

int RandomByScore(double* scores, int n) {
	int answer = 0;
	double sum = 0;
	for (int i = 0; i < n; i++) {
		sum += scores[i];
	}

	double randomAccumulate = (rand() / (double)0x00007FFF) * sum;

	sum = scores[0];
	for (int i = 1; i < n; sum += scores[i], i++) {
		if (randomAccumulate > sum) {
			answer = i;
		}
	}

	return answer;
}

void GeneticAlgorithm() {
	unsigned int Population[N];
	unsigned int Offspring[N];
	unsigned int NextGeneration[N];
	double Scores[2 * N];

	unsigned int BestX = (unsigned int)0;
	double BestY = -1.0;

	double currentScore;
	//初始随机种群
	for (int i = 0; i < N; i++) {
		Population[i] = (unsigned int)(rand() % MaxCode);
	}

	for (int t = 0; t < MaxIteration; t++) {
		printf("%d\n", t);
		//评估现有种群
		for (int i = 0; i < N; i++) {
			Scores[i] = EvaluateFunction(Population[i]);
			if (Scores[i] > BestY) {
				BestY = Scores[i];
				BestX = Population[i];
			}
		}

		//根据现有种群个体的分数（值大分高）选择生成后代
		for (int i = 0; i < N; i++) {
			Offspring[i] = GenerateOffspring(Population[RandomByScore(Scores, N)], Population[RandomByScore(Scores, N)]);
			Scores[N + i] = EvaluateFunction(Offspring[i]);
			if (Scores[N + i] > BestY) {
				BestY = Scores[N + i];
				BestX = Offspring[i];
			}
		}

		//从父代和子代中根据分数选出下一代
		for (int i = 0, index; i < N; i++) {
			index = RandomByScore(Scores, 2 * N);
			NextGeneration[i] = index < N ? Population[index] : Offspring[index % N];
		}

		for (int i = 0; i < N; i++) {
			Population[i] = NextGeneration[i];
		}
	}

	printf("BestX: %.3f\nBestY: %.3f\n", BestX * 0.0001, BestY);
}


int main2019_11_13() {
	srand((unsigned int)time(nullptr));
	GeneticAlgorithm();
	return 0;
}