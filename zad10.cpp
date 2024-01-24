#include <iostream>
#include <vector>
#include <random>
#include <limits>
#include <iomanip>
#include <cmath>
using namespace std;

#define SWARM_SIZE 50
#define W 0.729
#define C1 1.494
#define C2 1.494
#define VNMAX 0.7

random_device dev;
mt19937 rng(dev());
uniform_real_distribution<double> gen_real07(0, 7);
uniform_real_distribution<double> gen_real01(0, 1);

vector<double> A = { 1, 4, 0 };
vector<double> B = { 3, 2, 0 };
vector<double> C = { 2, 7, 1 };
vector<double> D = { 6, 3, 3 };
vector<double> E = { 7, 6, 5 };
vector<double> F = { 5, 7, 4 };

// S1(x1, y1, z1) S2(x2, y2, z2)

typedef struct vec_3d {
	double x; double y; double z;
} vec_3d;

double calc_distance(vec_3d a, vec_3d b) {
	return sqrt(
		(a.x - b.x) * (a.x - b.x)
		+ (a.y - b.y) * (a.y - b.y)
		+ (a.z - b.z) * (a.z - b.z)
	);
}


double opt_fun(const vector<double>& x) {
	return calc_distance({ A[0], A[1], A[2] }, { x[0], x[1], x[2] })
		+ calc_distance({ B[0], B[1], B[2] }, { x[0], x[1], x[2] })
		+ calc_distance({ C[0], C[1], C[2] }, { x[0], x[1], x[2] })
		+ calc_distance({ x[0], x[1], x[2] }, { x[3], x[4], x[5] })
		+ calc_distance({ D[0], D[1], D[2] }, { x[3], x[4], x[5] })
		+ calc_distance({ E[0], E[1], E[2] }, { x[3], x[4], x[5] })
		+ calc_distance({ F[0], F[1], F[2] }, { x[3], x[4], x[5] });
}

void init_swarm(vector<vector<double>>& swarm) {
	for (int i = 0; i < SWARM_SIZE; ++i)
		for (int j = 0; j < swarm[0].size(); ++j)
			swarm[i][j] = gen_real07(rng);
}

void calc_scores(const vector<vector<double>>& swarm, vector<double> &agent_scores, vector<double>& personal_bests,
	vector<vector<double>>& personal_best_positions, double& global_best, vector<double> &global_best_position) {
	for (int i = 0; i < SWARM_SIZE; ++i) {
		agent_scores[i] = opt_fun(swarm[i]);
		if (agent_scores[i] < personal_bests[i]) {
			personal_bests[i] = agent_scores[i];
			for (int j = 0; j < 6; ++j)
				personal_best_positions[i][j] = swarm[i][j];
		}
		if (personal_bests[i] < global_best) {
			global_best = personal_bests[i];
			for (int j = 0; j < 6; ++j)
				global_best_position[j] = swarm[i][j];
		}
	}
}

void next_positions(vector<vector<double>>& swarm, const vector<vector<double>>& personal_best_positions,
	vector<vector<double>> &vn, vector<double> &global_best_position) {
	// vn = w * vn-1 + c1 * rand() * (pbest - xn-1) + c2 * rand() * (gbest - xn-1)
	for (int i = 0; i < swarm.size(); ++i) {
		double rand1 = gen_real01(rng);
		double rand2 = gen_real01(rng);
		for (int j = 0; j < swarm[0].size(); ++j) {
			vn[i][j] = W * vn[i][j]
			+ C1 * rand1 * (personal_best_positions[i][j] - swarm[i][j])
			+ C2 * rand2 * (global_best_position[j] - swarm[i][j]);
			vn[i][j] = min(VNMAX, vn[i][j]);
		}
	}
	// xn = xn-1 + vn*delta(t)
	for (int i = 0; i < swarm.size(); ++i)
		for (int j = 0; j < swarm[0].size(); ++j)
			swarm[i][j] += vn[i][j];
}

int main() {

	vector<vector<double>> swarm(SWARM_SIZE, vector<double>(6));
	vector<vector<double>> vn(SWARM_SIZE, vector<double>(6, 0));
	vector<double> agent_scores(SWARM_SIZE);

	vector<double> personal_bests(SWARM_SIZE, numeric_limits<double>::max());
	vector<vector<double>> personal_best_positions(SWARM_SIZE, vector<double>(6));
	double global_best = numeric_limits<double>::max();
	vector<double> global_best_position(6);

	init_swarm(swarm);
	for (int i = 0; i < 100000; ++i) {
		calc_scores(swarm, agent_scores, personal_bests,
			personal_best_positions, global_best, global_best_position);
		next_positions(swarm, personal_best_positions, vn, global_best_position);
	}
	cout << setprecision(13);
	cout << "global_best: " << global_best << endl;
	cout << "S1(" << global_best_position[0] << "," << global_best_position[1] << "," << global_best_position[2] << ")" << endl;
	cout << "S2(" << global_best_position[3] << "," << global_best_position[4] << "," << global_best_position[5] << ")" << endl;

	return 0;
}