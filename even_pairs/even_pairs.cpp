#include <iostream>
#include <vector>

using namespace std;

vector<int> build_partial_sums(vector<int> v) {
	vector<int> partial_sums;
	partial_sums.push_back(v[0]);
	for (int i = 1; i < v.size(); i++) {
		partial_sums.push_back((partial_sums[i - 1] + v[i]) % 2);
	}
	return partial_sums;
}

void testcase_function() {
	int nr_numbers;
	cin >> nr_numbers;
	vector<int> v;
	int temp;
	for (int i = 0; i < nr_numbers; i++) {
		cin >> temp;
		v.push_back(temp);
	}
	vector<int> partial_sums = build_partial_sums(v);
	int even_count = 0;
	
	for (int j = 0; j < nr_numbers; j++) {
		if (partial_sums[j] == 0) {
			even_count++;
		}
	}
	int partial_even_count = even_count;
	int partial_odd_count = nr_numbers - partial_even_count;
	even_count += partial_even_count * (partial_even_count - 1) / 2;
	even_count += partial_odd_count * (partial_odd_count - 1) / 2;
	/*
	for (int i = 1; i < nr_numbers; i++) {
		for (int j = i; j < nr_numbers; j++) {
			if (partial_sums[j] - partial_sums[i - 1] == 0) {
				even_count++;
			}
		}
	}*/


	cout << even_count << endl;
}

int main() {
	int nr_cases;
	cin >> nr_cases;
	for (int i = 0; i < nr_cases; i++) {
		testcase_function();
	}

}