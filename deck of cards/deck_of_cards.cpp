#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>

using namespace std;

void testcase() {
	int n; cin >> n;
	int k; cin >> k;
	vector<int> v;
	for (int i = 0; i < n; i++) {
		int tmp; cin >> tmp;
		v.push_back(tmp);
	}

	int i = -1;
	int j = -1;

	// Calculate prefix sum
	vector<int> prefixSum;
	prefixSum.push_back(v[0]);
	for (int i = 1; i < n; i++) {
		prefixSum.push_back(prefixSum[i - 1] + v[i]);
	}

	auto val = [&prefixSum, &k](int i, int j) {
		if (i == 0) {
			return abs(k - prefixSum[j]);
		}
		else {
			return abs(k - prefixSum[j] + prefixSum[i - 1]);
		}
		
	};

	int bestVal = std::numeric_limits<int>::max();

	//for (int i1 = 0; i1 < n; i1++) {
	//	for (int j1 = 0; j1 < n; j1++) {
	//		int newVal = val(i1, j1);
	//		if (newVal < bestVal) {
	//			i = i1;
	//			j = j1;
	//			bestVal = newVal;
	//		}
	//	}
	//}

	for (int j1 = 0; j1 < n; j1++) {
		int newVal = val(0, j1);
		if (newVal < bestVal) {
			i = 0;
			j = j1;
			bestVal = newVal;
		}
	}

	for (int i1 = 1; i1 < n; i1++) {
		int target = k + prefixSum[i1-1];
		auto targetIndex = lower_bound(prefixSum.begin(), prefixSum.end(), target);
		int index = targetIndex - prefixSum.begin();
		if (index >= i1) {
			if (index < n) {
				int val2 = val(i1, index);
				if (val2 < bestVal) {
					i = i1;
					j = index;
					bestVal = val2;
				}
			}
			if (index > i1) {
				int val1 = val(i1, index - 1);
				if (val1 < bestVal) {
					i = i1;
					j = index - 1;
					bestVal = val1;
				}
			}
		}
	}

	cout << i << " " << j << endl;
}

int main() {
	int number_testcases; cin >> number_testcases;
	for (int i = 0; i < number_testcases; i++) {
		testcase();
	}
}