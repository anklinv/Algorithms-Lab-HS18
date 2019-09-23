#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

void testcasefunction() {
	int nr_numbers;
	cin >> nr_numbers;
	vector<int> v;
	int tmp;
	for (int i = 0; i < nr_numbers; i++) {
		cin >> tmp;
		v.push_back(tmp);
	}
	int maxindex = 1;
	for (int i = 0; i < nr_numbers; i++) {
		if (i >= maxindex) {
			cout << maxindex << endl;
			return;
		}
		maxindex = max(i + v[i], maxindex);
	}
	cout << nr_numbers << endl;
}

int main() {
	int nr_cases;
	cin >> nr_cases;
	for (int i = 0; i < nr_cases; i++) {
		testcasefunction();
	}
}