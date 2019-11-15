#include <iostream>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <algorithm>
using namespace std;

pair<int, int> RandomPair(int n) {
	int a = rand() % n;
	int b = a;
	while (b == a) {
		b = rand() % n;
	}
	return pair<int, int>(a, b);
}

double TSPCost(int n, int* cityOrder, double** adjacency) {
	double answer = 0.0;
	for (int i = 1; i < n; i++) {
		answer += adjacency[cityOrder[i - 1]][cityOrder[i]];
	}
	return answer;
}

double SimulatedAnnealingTSP(int n, int* cityOrder, double** adjacency, double optimalCost) {
	optimalCost *= 1.25;
	pair<int, int> switchPair;
	double T = 500.0, cost = TSPCost(n, cityOrder, adjacency), deltaCost;
	while (cost > optimalCost) {
		for (int i = 0; cost > optimalCost && i < 0x00002FFF; i++) {
			//SA
			cost = TSPCost(n, cityOrder, adjacency);
			switchPair = RandomPair(n);
			swap(cityOrder[switchPair.first], cityOrder[switchPair.second]);

			deltaCost = TSPCost(n, cityOrder, adjacency) - cost;
			printf("Cost: %.3f\tT: %.3f\t", cost, T);
			if (deltaCost >= 0) {
				printf("P: %.3f\n", exp(-(deltaCost / T)));
			}
			else {
				printf("\n");
			}
			if (deltaCost >= 0 && (rand() / (double)0x00007FFF) >= exp(-(deltaCost / T))) {
				swap(cityOrder[switchPair.first], cityOrder[switchPair.second]);
			}
		}
		T *= 0.99;
	}
	return cost;
}

double L2Norm(pair<double, double> x, pair<double, double> y) {
	return sqrt(pow(x.first - y.first, 2) + pow(x.second - y.second, 2));
}

int main_2019_11_13() {
	srand((unsigned int)time(nullptr));
	int n;
	double optimalCost;
	cin >> n >> optimalCost;
	double** adjacency = new double* [n];
	pair<double, double>* coordinate = new pair<double, double>[n];

	for (int i = 0, index; i < n; i++) {
		cin >> index;
		cin >> coordinate[index - 1].first >> coordinate[index - 1].second;
	}

	for (int i = 0; i < n; i++) {
		adjacency[i] = new double[n];
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			adjacency[i][j] = L2Norm(coordinate[i], coordinate[j]);
			adjacency[j][i] = adjacency[i][j];
		}
	}

	int* cityOrder = new int[n];
	for (int i = 0; i < n; i++) {
		cityOrder[i] = i;
	}

	double cost = SimulatedAnnealingTSP(n, cityOrder, adjacency, optimalCost);
	for (int i = 0; i < n; i++) {
		printf("%d\n", cityOrder[i]);
	}
	printf("Cost: %.3f\n", cost);
	return 0;
}