#include <iostream>
#include <cmath>
#include <random>
#include <fstream>
#include <sstream>
using namespace std;

int s[] = {
	173669, 275487, 1197613, 1549805, 502334, 217684, 1796841, 274708,
	631252, 148665, 150254, 4784408, 344759, 440109, 4198037, 329673, 28602,
	144173, 1461469, 187895, 369313, 959307, 1482335, 2772513, 1313997, 254845,
	486167, 2667146, 264004, 297223, 94694, 1757457, 576203, 8577828, 498382,
	8478177, 123575, 4062389, 3001419, 196884, 617991, 421056, 3017627, 131936,
	1152730, 2676649, 656678, 4519834, 201919, 56080, 2142553, 326263, 8172117,
	2304253, 4761871, 205387, 6148422, 414559, 2893305, 2158562, 465972, 304078,
	1841018, 1915571
};

std::random_device rand_dev;
std::mt19937 generator(rand_dev());
std::uniform_int_distribution<int> distr(0, 2);
std::random_device rd;
std::mt19937 e2(rd());
std::uniform_real_distribution<> dist(0, 1);
std::random_device randDev;
std::mt19937 gen(randDev());
std::uniform_int_distribution<int> hDist(1, 64);

int getRandInt0_2() { return distr(generator); }
int getRandInt1_64() { return hDist(gen); }
double gerRandDouble0_1() { return dist(e2); }

int calcF1(int x[64]) {
	int f1 = 1 << 25;
	for (int i = 0; i < 64; ++i)
		f1 -= (x[i] == 1) * s[i];
	return f1;
}

int calcF2(int x[64]) {
	int f2 = 1 << 25;
	for (int i = 0; i < 64; ++i)
		f2 -= (x[i] == 2) * s[i];
	return f2;
}

int calcf(int x[64]) {
	int f1 = calcF1(x);
	int f2 = calcF2(x);
	if (f1 < 0 || f2 < 0) return 1 << 26;
	else return f1 + f2;
}

double nextTi(int Tk) {
	return 0.95 * Tk;
}

double calcP(double dE, double Tk) {
	return exp(-dE / Tk);
}

double hammingDistance(int x1[64], int x2[64]) {
	double dist = 0;
	for (int i = 0; i < 64; ++i)
		dist += x1[i] == x2[i];
	return dist;
}

double iTot = 100000;
double hi(double i) {
	double hMin = 64;
	double hMax = 1;
	return (hMin - hMax) * (i - 1) / (iTot - 1) + hMax;
}

void createNewX(int x1[], int x2[], int iteration) {
	double h = hi(iteration);
	int changeFiledNum = getRandInt1_64() % (int)round(h);
	for (int i = 0; i < 64; ++i) x2[i] = x1[i];
	for (int i = 0; i < changeFiledNum; ++i)
		x2[getRandInt1_64() - 1] = getRandInt0_2();
}

int main() {

	double T0 = 64.0 * 1024 * 1024;


	int bestF = INT_MAX;
	int bestX[64];
	for (int i = 0; i < 20; ++i) {
		int x[64]; for (int i = 0; i < 64; x[i++] = getRandInt0_2());
		stringstream oss; oss << "rez" << i + 1 << ".txt";
		ofstream saveRezults(oss.str());
		int bestFLocal = INT_MAX;
		int bestXLocal[64];
		for (int k = 0; k < 10; ++k) {
			double T = T0;
			for (int i = 0; i < (int)iTot; ++i) {
				if (i == 0 && bestFLocal != INT_MAX) 
					for (int i = 0; i < 64; ++i) x[i] = bestXLocal[i];
				int fOfX = calcf(x);
				if (fOfX < bestF) {
					bestF = fOfX; for (int i = 0; i < 64; ++i) bestX[i] = x[i];
				}
				if (fOfX < bestFLocal) {
					bestFLocal = fOfX; for (int i = 0; i < 64; ++i) bestXLocal[i] = x[i];
				}
				saveRezults << fOfX << " ";
				int x2[64]; createNewX(x, x2, iTot - i);
				double xf = calcf(x), x2f = calcf(x2);
				double deltaF = x2f - xf;
				if (deltaF < 0 || gerRandDouble0_1() < calcP(deltaF, T))
					for (int i = 0; i < 64; ++i) x[i] = x2[i];
				T = nextTi(T);
			}
		}
	}
	cout << "best optimization function value = " << bestF << endl;
	cout << "x = ";
	for (int i = 0; i < 64; ++i) cout << bestX[i] << " ";
	cout << endl;
	return 0;
}