#include <vector>
#include <iostream>

using namespace std;

void testcase() {
	int nr_disks, target, k;
	cin >> nr_disks >> target >> k;
	
	vector<int> height(nr_disks);
	for (int i = 0 ; i < nr_disks ; ++i) {
		cin >> height[i];
	}
	
	// Create a DP table. An entry (i,j) means that with the first (i+1) disks it is possible to build a tower of a height % k = j.
	vector<vector<bool>> DP(nr_disks, vector<bool>(k, false));
	
	// With only the first piece, it is only possible to build a (non-empty) tower of height[0] % k, hence we set that entry to true.
	DP[0][height[0] % k] = true;
	
	for (int i = 1; i < nr_disks; ++i) {
		for (int j = 0; j < k; ++j) {
			// For each added piece, we can either take it or not. We set the entry to true if either of these choices is possible.
			DP[i][j] = DP[i-1][j] || // If we do not take it, we can just lookup the value before
					   (height[i] % k) == j || // If we take it on its own
					   DP[i-1][abs(j - height[i]) % k]; // If we take it and place it on top of a different tower
		}
	}
	
	// Check if the desired entry is set to true.
	if (DP[nr_disks-1][target]) {
		cout << "yes" << endl;
	} else {
		cout << "no" << endl;
	}
}

int main() {
	int t; cin >> t;
	while(t--)
		testcase();
}