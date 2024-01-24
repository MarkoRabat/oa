#include <algorithm>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <cmath>
using namespace std;

class City {
public:
	int getCityIndex() const { return cityIndex; }
	double getX() const { return x; }
	double getY() const { return y; }
	bool operator<(City const& otherCity) {
		return cityIndex < otherCity.cityIndex; }
	bool operator>(City const& otherCity) {
		return cityIndex > otherCity.cityIndex; }
	bool operator==(City const& otherCity) {
		return cityIndex == otherCity.cityIndex; }
	bool operator!=(City const& otherCity) {
		return cityIndex != otherCity.cityIndex; }
	double operator-(City const& otherCity) {
		if (cachedDistances[cityIndex][otherCity.cityIndex] >= 0.0)
			return cachedDistances[cityIndex][otherCity.cityIndex];
		double res = sqrt(
			(x - otherCity.x) * (x - otherCity.x)
			+ (y - otherCity.y) * (y - otherCity.y));
		cachedDistances[cityIndex][otherCity.cityIndex] = 
			cachedDistances[otherCity.cityIndex][cityIndex] = res;
		return res;
	}
	static vector<City> createVectorOfCitys(
			const vector<vector<double>> &coordinates);
private:
	City(double x, double y) {
		this->cityIndex = currCityIndex++;
		this->x = x;
		this->y = y;
		for (int i = 0; i < cachedDistances.size(); ++i)
			cachedDistances[i].push_back(-1.0);
		cachedDistances.emplace_back(vector<double>(currCityIndex));
		for (int i = 0; i < cachedDistances[currCityIndex - 1].size(); ++i)
			cachedDistances[currCityIndex - 1][i] = -1.0;
	}
	static vector<vector<double>> cachedDistances;
	static void saveCachedDistances();
	static bool loadCachedDistances();
	static int currCityIndex;
	int cityIndex;
	double x;
	double y;
};
vector<vector<double>> City::cachedDistances;
int City::currCityIndex;
void City::saveCachedDistances() {
	ofstream saveTo("cachedDistanceMatrix.txt");
	saveTo << setprecision(15);
	for (int i = 0; i < currCityIndex; ++i) {
		for (int j = 0; j < currCityIndex; ++j) {
			saveTo << cachedDistances[i][j];
			if (j != currCityIndex - 1) saveTo << " ";
		}
		if (i != currCityIndex - 1) saveTo << endl;
	}
	saveTo.close();
}
bool City::loadCachedDistances() {
	ifstream loadFrom("cachedDistanceMatrix.txt");
	if (!loadFrom.good()) {
		cerr << "Couldn't load values from file cachedDistanceMatrix.txt; file doesn't exist" << endl; return false; }
	vector<vector<double>> loadedCachedDist(currCityIndex, vector<double>(currCityIndex));
	for (int i = 0; i < currCityIndex; ++i) {
		for (int j = 0; j < currCityIndex; ++j) {
			double next; loadFrom >> next;
			if (!loadFrom) {
				loadFrom.close();
				cerr << "Couldn't load values from file cachedDistanceMatrix.txt;" << endl 
					 << "         -- size of the file is incompatible with number of points:" << endl
					 << "         number of values in the file is lower then current number of points created" << endl;
				return false;
			}
			loadedCachedDist[i][j] = next;
		}
	}
	double checkEof; loadFrom >> checkEof;
	if (loadFrom) {
		cerr << "Couldn't load values from file cachedDistanceMatrix.txt;" << endl
			<< "         -- size of the file is incompatible with number of points:" << endl
			<< "         number of values in the file is greater then current number of points created" << endl;
		loadFrom.close(); return false;
	}
	else cachedDistances = move(loadedCachedDist);
	loadFrom.close();
	return true;
}

vector<City> City::createVectorOfCitys(const vector<vector<double>> &coordinates) {
	vector<City> cities;
	for (int i = 0; i < coordinates.size(); ++i)
		cities.emplace_back(City(coordinates[i][0], coordinates[i][1]));
	if (!loadCachedDistances()) {
		// create and save cachedDistance matrix
		for (int i = 0; i < currCityIndex; ++i)
			for (int j = 0; j < currCityIndex; ++j)
				cities[i] - cities[j];
		saveCachedDistances();
	}
	return cities;
}

int main() {
	vector<City> cities = City::createVectorOfCitys({
		{2.7, 33.1}, {2.7, 56.8}, {9.1, 40.3}, {9.1, 52.8},
		{15.1, 49.6}, {15.3, 37.8}, {21.5, 45.8}, {22.9, 32.7},
		{33.4, 60.5}, {28.4, 31.7}, {34.7, 26.4}, {45.7, 25.1},
		{34.7, 45.1}, {46.0, 45.1}, {54.2, 29.1}, {57.7, 42.1},
		{67.9, 19.6}, {51.7, 56.0}, {57.5, 56}, {62.0, 58.4},
	});
	return 0;
}