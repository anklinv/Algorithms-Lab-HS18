#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

void testcase() {
	int n; cin >> n;
	vector<int> times(n, -1);
	for(int i = 0; i < n; ++i) {
		cin >> times[i];
	}

	// Extract min
	vector<int> mins(n);
	mins[0] = times[0];
	for(int j = 0; j <= (n-3)/2; ++j) {
		// cout << "j = " << j << endl;
		mins[2*j + 1] = min(mins[j], times[2*j + 1]);
		mins[2*j + 2] = min(mins[j], times[2*j + 2]);
	}

	// Sort mins
	sort(mins.begin(), mins.end());
	
	bool boom = false;
	// Validate sorted mins
	for (int i = 0; i < n; ++i) {
		if (i >= mins[i]) { // = due to 0 indexed
			boom = true;
			break;
		}
	}
	
	if (boom) {
		cout << "no" << endl;
	} else {
		cout << "yes" << endl;
	}
}

int main() {
	int t; cin >> t;
	while(t--)
		testcase();
}