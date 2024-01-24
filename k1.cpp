#include <iostream>
#include <random>
#include <vector>
#include <cmath>
using namespace std;

#define CALC_ROW(N, i) (i) / (N)
#define CALC_COLUMN(N, i) (i) % (N)

int optimizationFunction(const vector<int>& x0, int N,
	vector<int> R, vector<int> C) {
	for (int i = 0; i < N * N; ++i) {
		R[CALC_ROW(N, i)] -= x0[i];
		C[CALC_COLUMN(N, i)] -= x0[i];
	}
	int rez = 0;
	for (int i = 0; i < N; ++i) {
		rez += abs(R[i]) + abs(C[i]);
	}
	return rez;
}

int optimizationFunction(int x0[], int N,
	vector<int> R, vector<int> C) {
	for (int i = 0; i < N * N; ++i) {
		R[CALC_ROW(N, i)] -= x0[i];
		C[CALC_COLUMN(N, i)] -= x0[i];
	}
	int rez = 0;
	for (int i = 0; i < N; ++i) {
		rez += abs(R[i]) + abs(C[i]);
	}
	return rez;
}

// f(x) = sum | R(i + 1) - (sum x(i*N + j)| + sum | C(i) - (sum x(j * N + i)) |
//	   i=0 to N-1      j=1 to N			    i=1 to N     j=0 to N - 1

void variations_with_repetition_zad3(int n, int k, int N, const vector<int> &R, const vector<int> &C) {
	int q;
	int* P = new int [k];
	for (int i = 0; i < k; ++i) P[i] = 0;
	do {

		if (!optimizationFunction(P, N, R, C)) {
			for (int i = 0; i < k; ++i) printf("%d ", P[i]);
			printf(" optimizaciona funkcija: %d\n", optimizationFunction(P, N, R, C));
		}
		q = k - 1;
		while (q >= 0) {
			P[q]++;
			if (P[q] == n) {
				P[q] = 0;
				--q;
			}
			else break;
		}
	} while (q >= 0);
	delete [] P;
}


void zadatak4() {
	int N = 8;
	vector<int> R = { 7, 3, 3, 4, 3, 3, 7, 0, };
	vector<int> C = { 2, 7, 7, 3, 5, 2, 4, 0, };
	cout << "\nzadatak4: " << endl;
	std::random_device rd;
	std::mt19937 mt(rd());

	std::uniform_int_distribution<int> random_int(0, 1);
	vector<int> best;
	int best_score = INT_MAX;
	for (int i = 0; i < 10000000; ++i) {
		vector<int> curr(N * N);
		for (int i = 0; i < N * N; ++i)
			curr[i] = random_int(mt);
		int score = optimizationFunction(curr, N, R, C);
		if (score < best_score) {
			best_score = score; best = curr; }
	}

	cout << "najbolji rezultat:\n";
	for (int e : best) cout << e << " ";
	cout << "\n vrednost optimizacione funkcije za najbolji rezultat: " << best_score << endl;

	/*
	0 1 1 1 1 1 1 1 
	1 1 1 0 0 0 0 0 
	0 1 1 0 0 0 1 0 
	0 1 0 0 1 0 0 1 
	1 0 1 0 1 1 1 1
	0 1 1 0 1 0 0 0 
	1 1 1 1 1 0 1 0 
	0 0 0 1 0 0 0 0
	*/
}

void zadatak3() {
	int N = 5;
	vector<int> R = { 5, 3, 3, 0, 2, };
	vector<int> C = { 4, 4, 2, 2, 1, };
	cout << "\nzadatak3: " << endl;
	variations_with_repetition_zad3(2, N * N, N, R, C);


	/* 1 1 1 1 1
       1 1 0 1 0
	   1 1 1 0 0 
	   0 0 0 0 0 
	   1 1 0 0 0  optimizaciona funkcija : 0 */
	/* 1 1 1 1 1
	   1 1 1 0 0 
	   1 1 0 1 0 
	   0 0 0 0 0 
	   1 1 0 0 0  optimizaciona funkcija : 0 */

}


void zadatak2() {
	int N = 5;
	vector<int> R = { 5, 3, 3, 0, 2 };
	vector<int> C = { 4, 4, 2, 2, 1 };
	vector<int> x0 = {
		0, 1, 1, 1, 1,
		1, 0, 1, 0, 0,
		1, 1, 0, 1, 0,
		0, 0, 1, 0, 0,
		1, 1, 0, 0, 1,
	};
	cout << "\nzadatak2: \n" << "optimizaciona funkcija za x0: ";
	cout << optimizationFunction(x0, N, R, C) << endl;
}

int main() {

	zadatak2();
	zadatak3();
	zadatak4();

	return 0;
}