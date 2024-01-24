#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <random>
using namespace std;

#define POPULATION_SIZE 20000
#define NUMBER_OF_GENERATIONS 50
#define INDEPENDENT_ITER_NUM 20
#define MUTATION_RATE 0.9
#define SELECT_FOR_MUTATION 0.15

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

random_device dev;
mt19937 rng(dev());
uniform_int_distribution<mt19937::result_type> dist(0, 2);
uniform_int_distribution<mt19937::result_type> chose_unit(0, POPULATION_SIZE - 1);
uniform_int_distribution<mt19937::result_type> chose_parent(0, POPULATION_SIZE / 5 - 1);
uniform_int_distribution<mt19937::result_type> chose_crossover_point(0, sizeof(s) / sizeof(int));
uniform_real_distribution<double> rand_double(0, 1);


int calc_f1(const vector<int> &x) {
	int f1 = 1 << 25;
	for (int i = 0; i < 64; ++i)
		f1 -= (x[i] == 1) * s[i];
	return f1;
}

int calc_f2(const vector<int> &x) {
	int f2 = 1 << 25;
	for (int i = 0; i < 64; ++i)
		f2 -= (x[i] == 2) * s[i];
	return f2;
}

int calc_f(const vector<int> &x) {
	int f1 = calc_f1(x);
	int f2 = calc_f2(x);
	if (f1 < 0 || f2 < 0) return 1 << 26;
	else return f1 + f2;
}

void initialize_population(vector<vector<int>>& population) {
	for (int i = 0; i < POPULATION_SIZE; ++i) {
		vector<int> individual(sizeof(s) / sizeof(int));
		for (int i = 0; i < sizeof(s) / sizeof(int); individual[i++] = dist(rng));
		population.emplace_back(individual);
	}
}

void evaluate_population(vector<int>& population_reverse_fitness,
	const vector<vector<int>> &population) {
	for (int i = 0; i < population.size(); ++i)
		population_reverse_fitness[i] = calc_f(population[i]);
}

void tournament_selection(vector<int> &selected_indices,
	const vector<int> &population_reverse_fitness) {
	int selection_size = POPULATION_SIZE / 5;
	for (int i = 0; i < selection_size; ++i) {
		int unit1_index, unit2_index; bool chose_again;
		do {
			unit1_index = chose_unit(rng); chose_again = false;
			for (int j = 0; j < selected_indices.size(); ++j)
				if (unit1_index == selected_indices[j]) { chose_again = true; break; }
		} while (chose_again);
		do {
			unit2_index = chose_unit(rng); chose_again = false;
			if (unit2_index == unit1_index) { chose_again = true;  continue; }
			for (int j = 0; j < selected_indices.size(); ++j)
				if (unit2_index == selected_indices[j]) { chose_again = true; break; }
		} while (chose_again);
		if (population_reverse_fitness[unit1_index] < population_reverse_fitness[unit2_index])
			selected_indices.push_back(unit1_index);
		else selected_indices.push_back(unit2_index);
	}
}

void decimation_selection(vector<int> &selected_indices,
	vector<int> &population_reverse_fitness) {
	int selection_size = POPULATION_SIZE / 30;
	sort(population_reverse_fitness.begin(), population_reverse_fitness.end());
	for (int i = 0; i < selection_size; ++i)
		selected_indices.push_back(i);
}

void binary_crossover(vector<int>& selected_indices, vector<vector<int>>& population) {
	double crossover_probability = 0.8;
	vector<vector<int>> new_population;

	while (new_population.size() < POPULATION_SIZE) {
		int parent1_index = chose_parent(rng);
		int parent2_index = chose_parent(rng);
		if (rand_double(rng) < crossover_probability) continue;

		vector<int> new_unit1(sizeof(s) / sizeof(int));
		vector<int> new_unit2(sizeof(s) / sizeof(int));
		int crossover_point = chose_crossover_point(rng);
		for (int i = 0; i < sizeof(s) / sizeof(int); ++i) {
			if (i < crossover_point) {
				new_unit1[i] = population[parent1_index][i];
				new_unit2[i] = population[parent2_index][i];
			}
			else {
				new_unit1[i] = population[parent2_index][i];
				new_unit2[i] = population[parent1_index][i];
			}
		}
		new_population.emplace_back(new_unit1);
		new_population.emplace_back(new_unit2);
	}
	move(new_population.begin(), new_population.end(), population.begin());
}

void ternary_crossover(vector<int>& selected_indices, vector<vector<int>>& population) {
	double crossover_probability = 0.8;
	vector<vector<int>> new_population;

	while (new_population.size() < POPULATION_SIZE) {
		int parent1_index = chose_parent(rng);
		int parent2_index = chose_parent(rng);
		if (rand_double(rng) < crossover_probability) continue;

		vector<int> new_unit1(sizeof(s) / sizeof(int));
		vector<int> new_unit2(sizeof(s) / sizeof(int));
		int crossover_point1 = chose_crossover_point(rng);
		int crossover_point2 = chose_crossover_point(rng);
		if (crossover_point1 > crossover_point2) {
			int tmp = crossover_point1;
			crossover_point1 = crossover_point2;
			crossover_point2 = tmp;
		}
		for (int i = 0; i < sizeof(s) / sizeof(int); ++i) {
			if (i < crossover_point1) {
				new_unit1[i] = population[parent1_index][i];
				new_unit2[i] = population[parent2_index][i];
			}
			else if (i < crossover_point2){
				new_unit1[i] = population[parent2_index][i];
				new_unit2[i] = population[parent1_index][i];
			}
			else {
				new_unit1[i] = population[parent1_index][i];
				new_unit2[i] = population[parent2_index][i];
			}
		}
		new_population.emplace_back(new_unit1);
		new_population.emplace_back(new_unit2);
	}
	move(new_population.begin(), new_population.end(), population.begin());
}

void ternary_crossover_with_elitism(vector<int>& selected_indices, vector<vector<int>>& population) {
	double crossover_probability = 0.8;
	vector<vector<int>> new_population;
	for (int i = 0; i < selected_indices.size(); ++i)
		new_population.push_back(population[selected_indices[i]]);

	while (new_population.size() < POPULATION_SIZE) {
		int parent1_index = chose_parent(rng);
		int parent2_index = chose_parent(rng);
		if (rand_double(rng) < crossover_probability) continue;

		vector<int> new_unit1(sizeof(s) / sizeof(int));
		vector<int> new_unit2(sizeof(s) / sizeof(int));
		int crossover_point1 = chose_crossover_point(rng);
		int crossover_point2 = chose_crossover_point(rng);
		if (crossover_point1 > crossover_point2) {
			int tmp = crossover_point1;
			crossover_point1 = crossover_point2;
			crossover_point2 = tmp;
		}
		for (int i = 0; i < sizeof(s) / sizeof(int); ++i) {
			if (i < crossover_point1) {
				new_unit1[i] = population[parent1_index][i];
				new_unit2[i] = population[parent2_index][i];
			}
			else if (i < crossover_point2){
				new_unit1[i] = population[parent2_index][i];
				new_unit2[i] = population[parent1_index][i];
			}
			else {
				new_unit1[i] = population[parent1_index][i];
				new_unit2[i] = population[parent2_index][i];
			}
		}
		new_population.emplace_back(new_unit1);
		new_population.emplace_back(new_unit2);
	}
	move(new_population.begin(), new_population.end(), population.begin());
}

void mutate(vector<vector<int>> &population) {
	for (int i = 0; i < population.size(); ++i) {
		if (rand_double(rng) < SELECT_FOR_MUTATION) {
			for (int j = 0; j < population[i].size(); ++j)
				if (rand_double(rng) < MUTATION_RATE)
					population[i][j] = dist(rng);
		}
	}
}

int main() {

	int min_optf = INT_MAX;
	vector<int> best_unit;
	for (int rerun = 0; rerun < INDEPENDENT_ITER_NUM; ++rerun) {
		int curr_min_optf = INT_MAX;
		vector<vector<int>> population;
		vector<int> population_reverse_fitness(POPULATION_SIZE);
		vector<int> curr_best_unit;

		ofstream curr_file;
		curr_file.open(string("file") + to_string(rerun) + ".txt");

		initialize_population(population);

		for (int curr_gen = 0; curr_gen < 50; ++curr_gen) {

			evaluate_population(population_reverse_fitness, population);

			for (int opt_fun_val : population_reverse_fitness) {
				curr_file << opt_fun_val << " ";
			}

			int min_unit_index = min_element(population_reverse_fitness.begin(),
				population_reverse_fitness.end()) - population_reverse_fitness.begin();
			if (curr_min_optf > population_reverse_fitness[min_unit_index]) {
				curr_min_optf = population_reverse_fitness[min_unit_index];
				curr_best_unit = population[min_unit_index];
			}

			vector<int> selected_indeces;
			//tournament_selection(selected_indeces, population_reverse_fitness);
			decimation_selection(selected_indeces, population_reverse_fitness);


			//binary_crossover(selected_indeces, population);
			//ternary_crossover(selected_indeces, population);
			ternary_crossover_with_elitism(selected_indeces, population);
			mutate(population);
		}
		if (min_optf > curr_min_optf) {
			min_optf = curr_min_optf;
			best_unit = curr_best_unit;
		}
		curr_file.close();
		std::cout << "Min unit from curr iteration: " << curr_min_optf << endl;
	}
	std::cout << "---------------------------------";
	std::cout << "Min unit from all iterations: " << min_optf << endl;
	for (int gene : best_unit)
		std::cout << gene << " ";
	std::cout << endl;
	std::cout << "---------------------------------";
	return 0;
}