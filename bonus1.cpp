#include <iostream>
using namespace std;

bool checkCondition(int arr[72]) {
	for (int k = 56; k >= 0; --k)
		if (arr[k] + arr[k + 1] + arr[k + 2] + arr[k + 3]
			+ arr[k + 4] + arr[k + 5] + arr[k + 6] + arr[k + 7]
			>= arr[k + 8] + arr[k + 9] + arr[k + 10] + arr[k + 11]
			+ arr[k + 12] + arr[k + 13] + arr[k + 14] + arr[k + 15]) {
			cout << k << endl;
			cout << arr[k] << " " << arr[k + 1] << " " << arr[k + 2] << " " << arr[k + 3]
				<< " " << arr[k + 4] << " " << arr[k + 5] << " " << arr[k + 6] << " " << arr[k + 7] << endl;
			cout << arr[k + 8] << " " << arr[k + 9] << " " << arr[k + 10] << " " << arr[k + 11]
				<< " " << arr[k + 12] << " " << arr[k + 13] << " " << arr[k + 14] << " " << arr[k + 15] << endl;
			return false;
		}
	return true;
}


int main() {

	int niz[72] = {
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 1,
		0, 0, 0, 0, 0, 0, 1, 1,
		0, 0, 0, 0, 0, 1, 1, 1,
		0, 0, 0, 0, 1, 1, 1, 1,
		0, 0, 0, 1, 1, 1, 1, 1,
		0, 0, 1, 1, 1, 1, 1, 1,
		0, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1,
	};

	cout << checkCondition(niz) << endl;

	return 0;
}