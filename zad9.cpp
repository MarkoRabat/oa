#include <iostream>
#include <vector>
#include <random>
using namespace std;

#define POPULATION_SIZE 50
#define F 0.8
#define CR 0.9

default_random_engine re;
uniform_real_distribution<double> unif(-15, 15);
uniform_real_distribution<double> rand_double01(0, 1);
random_device dev;
mt19937 rng(dev());
uniform_int_distribution<mt19937::result_type> rand_index(0, POPULATION_SIZE - 1);
uniform_int_distribution<mt19937::result_type> return_r(0, 5);
double randp_from_sspace() { return unif(re); }
double get_rand_double01() { return rand_double01(re); }
int get_random_index() { return rand_index(rng); }
int get_r() { return return_r(rng); }

vector<double> S = {
	-0.09940112332480822, -0.09570265063923192, -0.07782620994584906,
	-0.044595775065571636, -0.008470411838648773, -0.0013292572938769093,
	-0.01402876134848341, 0.0011785680597112547, -0.0016096599564817682,
	-0.03141072397571561, -0.05773121434057853, -0.07098734083487862,
	-0.07421256224434619, -0.09674779542915338, -0.13216942328836218,
	-0.18406033359301877, -0.24214426775005213, -0.25978279767024376,
	-0.2186443973931424, -0.3289283483195699, -0.4205252223787085,
	-0.32130499477499636, -0.205134242990832, -0.13760381018149595
};

vector<double> pointsX = {
	-15, -10, -5, 0, 5, 10, 15, 15, 15, 15, 15, 15, 15, 10, 5, 0, -5, -10, -15, -15, -15, -15, -15, -15,
};

vector<double> pointsY = {
	-15, -15, -15, -15, -15, -15, -15, -10, -5, 0, 5, 10, 15, 15, 15, 15, 15, 15, 15, 10, 5, 0, -5, -10,
};

double opt_fun(vector<double> x) {
	double sum = 0;
	for (int i = 0; i < S.size(); ++i) {
		double currSum =
		  x[4] / ((pointsX[i] - x[0]) * (pointsX[i] - x[0]) + (pointsY[i] - x[1]) * (pointsY[i] - x[1]))
		+ x[5] / ((pointsX[i] - x[2]) * (pointsX[i] - x[2]) + (pointsY[i] - x[3]) * (pointsY[i] - x[3]))
		- S[i];
		sum += currSum * currSum;
	}
	return sum;
}

void init_population(vector<vector<double>>& population) {
	for (int i = 0; i < POPULATION_SIZE; ++i) {
		vector<double> new_unit;
		for (int j = 0; j < 6; ++j)
			new_unit.push_back(randp_from_sspace());
		population.emplace_back(new_unit);
	}
}

void calc_z(vector<double>& z, int i, const vector<vector<double>>& population) {
	int index1, index2, index3;
	while ((index1 = get_random_index()) == i);
	while ((index2 = get_random_index()) == i || index2 == index1);
	while ((index3 = get_random_index()) == i || index3 == index2 || index3 == index1);

	for (int i = 0; i < 6; ++i)
		z[i] = population[index1][i] + F * (population[index2][i] - population[index3][i]);

	if (z[0] >= -15 && z[0] <= 15)
	if (z[1] >= -15 && z[1] <= 15)
	if (z[2] >= -15 && z[2] <= 15)
	if (z[3] >= -15 && z[3] <= 15)
		return;
	calc_z(z, i, population);
}

void generate_next_population(vector<vector<double>>& population) {
	vector<vector<double>> new_population;
	for (int i = 0; i < POPULATION_SIZE; ++i) {
		vector<double> z;
		calc_z(z, i, population);
		int R = get_r();
		vector<double> y;
		for (int j = 0; j < 6; ++j) {
			if (j == R || get_rand_double01() < CR)
				y.push_back(z[j]);
			else y.push_back(population[i][j]);
		}
		if (opt_fun(y) < opt_fun(population[i]))
			new_population.emplace_back(y);
		else new_population.push_back(population[i]);
	}
	population = new_population;
}

int main() {
	// x = (xp1, yp1, xp2, yp2, A1, A2)
	// Sk = Ak / rk

	vector<vector<double>> population;
	init_population(population);
	cout << population[0][1] << endl;
	for (int i = 0; i < 5; ++i) {
		cout << opt_fun(population[i]) << endl;
	}
	generate_next_population(population);
	for (int i = 0; i < 5; ++i) {
		cout << opt_fun(population[i]) << endl;
	}



	return 0;
}