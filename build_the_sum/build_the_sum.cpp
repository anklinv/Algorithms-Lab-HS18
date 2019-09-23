#include <iostream>
#include <vector>

using namespace std;

void testcase() {
	int nr_numbers;
	cin >> nr_numbers;
	int sum = 0;
	int tmp;
	for (int i = 0; i < nr_numbers; i++) {
		cin >> tmp;
		sum += tmp;
	}
	cout << sum << endl;
}

int main() {
	int number_testcases; cin >> number_testcases;
	for (int i = 0; i < number_testcases; i++) {
		testcase();
	}
}