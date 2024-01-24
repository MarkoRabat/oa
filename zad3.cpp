#include <iostream>
#include <cstdlib>
#include <vector>
using namespace std;

int getDistance(int node1, int node2) {
	static int priceTable[10][10] = {
		{0, 374, 350, 223, 108, 178, 252, 285, 240, 356},
		{374, 0, 27, 166, 433, 199, 135, 95, 136, 17},
		{350, 27, 0, 41, 52, 821, 180, 201, 131, 247},
		{223, 166, 41, 0, 430, 47, 52, 84, 40, 155},
		{108, 433, 52, 430, 0, 453, 478, 344, 389, 423},
		{178, 199, 821, 47, 453, 0, 91, 37, 64, 181},
		{252, 135, 180, 52, 478, 91, 0, 25, 83, 117},
		{285, 95, 201, 84, 344, 37, 25, 0, 51, 42},
		{240, 136, 131, 40, 389, 64, 83, 51, 0, 118},
		{356, 17, 247, 155, 423, 181, 117, 42, 118, 0}
	};
	return priceTable[node1][node2];
}

void SequenceToSpanningTree(int* P, int len, int* T) {
	int* V = new int[len + 2]{};
	for (int i = 0; i < len; ++V[P[i++] - 1]);

	int q = 0;
	for (int i = 0; i < len; ++i)
		for (int j = 0; j < len + 2; ++j)
			if (V[j] == 0) {
				V[j] = -1; T[q++] = j + 1;
				T[q++] = P[i]; --V[P[i] - 1];
				break; }

	int j = 0;
	for (int i = 0; i < len + 2; ++i) {
		if (V[i] != 0) continue;
		if (j == 0) { T[q++] = i + 1; j++; }
		else if (j == 1) { T[q++] = i + 1; break; }
	}
	delete [] V;
}

void printSpanningTreeNodeLetters(int* T, int len) {
	for (int i = 0; i < 2 * (len + 1); ++i) {
		printf(" %c", T[i] + 'A' - 1);
		if ((i + 1) % 2 == 0 && i < 2 * len)
			printf("  - ");
	}
}


int optimizationFunction(int* T, int len, int currBest) {
	int cost = 0;
	int* P = new int[len + 2]{};

	for (int i = 0; i < 2 * (len + 1); i += 2) {
		cost += getDistance(T[i] - 1, T[i + 1] - 1);
		++P[T[i] - 1]; ++P[T[i + 1] - 1];
	}
	for (int i = 0; i < len + 2; ++i) {
		if (P[i] > 3) cost += 250 * (P[i] - 3);
	}

	delete[] P;
	return cost;
}

int* genAllVarWithRep(int n, int k, int &minPathLength) {
	int* P = new int[k];
	for (int i = 0; i < k; ++i) P[i] = 1;
	int* minVar = new int[k];
	minPathLength = INT_MAX;
	int* T = new int[2 * (k + 1)];
	int q;
	do {
		SequenceToSpanningTree(P, k, T);
		int currPathLength = optimizationFunction(T, k, minPathLength);
		if (currPathLength < minPathLength) {
			minPathLength = currPathLength;
			for (int i = 0; i < k; ++i) minVar[i] = P[i];
		}

		q = k - 1;
		while (q >= 0) {
			++P[q];
			if (P[q] != n + 1) break;
			P[q--] = 1;
		}
	} while (q >= 0);
	delete[] P;
	delete[] T;
	return minVar;
}

int main() {

	int minPathLength; int* T = new int[18];
	int* minPath = genAllVarWithRep(10, 8, minPathLength);
	SequenceToSpanningTree(minPath, 8, T);
	printSpanningTreeNodeLetters(T, 8);
	cout << "\n"; cout << minPathLength << endl;

	return 0;
}