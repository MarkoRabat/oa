#include <algorithm>
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

class City {
public:
	City(double x, double y) {
		this->cityIndex = ++currCityIndex;
		this->x = x;
		this->y = y;
	}
	int getCityIndex() const { return cityIndex; }
	double getX() const { return x; }
	double getY() const { return y; }
	bool operator<(City const& otherCity) {
		return cityIndex < otherCity.cityIndex;
	}
	bool operator==(City const& otherCity) {
		return cityIndex == otherCity.cityIndex;
	}
	double operator-(City const& otherCity) {
		if (cachedDistances[cityIndex][otherCity.cityIndex]) {
			return cachedDistances[cityIndex][otherCity.cityIndex];
		}
		double res = sqrt(
			(x - otherCity.x) * (x - otherCity.x)
			+ (y - otherCity.y) * (y - otherCity.y));
		cachedDistances[cityIndex][otherCity.cityIndex]
			= cachedDistances[otherCity.cityIndex][cityIndex] = res;
		return res;
	}

private:
	static double cachedDistances[12][12];
	static int currCityIndex;
	int cityIndex;
	double x;
	double y;
};
int City::currCityIndex;
double City::cachedDistances[12][12];


vector<City> createVectorOfCitys8() {
	vector<City> cities = {
		City(2.7, 33.1),
		City(2.7, 56.8),
		City(9.1, 40.3),
		City(9.1, 52.8),
		City(15.1, 49.6),
		City(15.3, 37.8),
		City(21.5, 45.8),
		City(22.9, 32.7),
	};
	return cities;
}

void add4Cities(vector<City>& cities) {
	cities.emplace_back(City(33.4, 60.5));
	cities.emplace_back(City(28.4, 31.7));
	cities.emplace_back(City(34.7, 26.4));
	cities.emplace_back(City(45.7, 25.1));
}

ostream& printCities(const vector<City>& cities) {
	for (int i = 0; i < cities.size(); ++i)
		cout << "(" << cities[i].getCityIndex() << ", "
			 << cities[i].getX() << ", "
			 << cities[i].getY() << ")" << " ";
	return cout;
}

ostream& printCitiIndexes(const vector<City>& cities) {
	for (int i = 0; i < cities.size(); ++i)
		cout << cities[i].getCityIndex() << " ";
	return cout;
}

double findTspMinPath(vector<City>& cities) {
	vector<City> tspMinPath;
	double minDistance = INFINITY;
	do {
		double currDistance = 0;
		for (int i = 1; i < cities.size() && currDistance < minDistance; ++i)
			currDistance += cities[i] - cities[i - 1];

		if (currDistance < minDistance) {
			tspMinPath = cities; minDistance = currDistance; }
	} while (next_permutation(cities.begin(), cities.end()));
	cities = tspMinPath;
	return minDistance;
}

int main() {
	vector<City> cities = createVectorOfCitys8();
	cout << "Min path length for 8 cities: " << findTspMinPath(cities) << "\n"
		 << "Path: "; printCitiIndexes(cities) << endl;
	add4Cities(cities);
	cout << "Min path length for 12 cities: " << findTspMinPath(cities) << "\n"
		 << "Path: "; printCitiIndexes(cities) << endl;

	return 0;
}