#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void testcase() {
	int n, k, x;
	cin >> n >> k >> x;
	
	// Build vector of initial state
	vector<bool> state(n);
	for (int i = 0; i < n; ++i) {
		int tmp; cin >> tmp;
		if (tmp) state[i] = true;
		else state[i] = false;
	}
	
	// Build vector of pattern
	vector<bool> pattern(k);
	for (int i = 0; i < k; ++i) {
		if (x bitand 1<<i)
			pattern[k - i - 1] = true;
		else
			pattern[k - i - 1] = false;
	}
	
	
	int nr_groups = n / k;
	int nr_swapped = 1; // start with one swap of everything
	int nr_switched = 0; // start with initial state
	
	
	// Iterate backwards through the groups
	for (int i = nr_groups; i > 0; --i) {
		
		int needToSwitch = 0;
		int needToSwitchIfSwapped = 0;
		
		// Compare pattern with current group
		for (int j = 0; j < k; ++j) {
			if (state[(i-1)*k + j] != pattern[j])
				needToSwitch++;
			else
				needToSwitchIfSwapped++;
		}
				
		// Go through cases that start with swapped state
		int new_nr_swapped = min(
			// We start with the swapped state
			min(
				// We just individually switch the lights
				nr_swapped + needToSwitchIfSwapped,
				
				// We first swap, then switch the individual lights, then swap back
				nr_swapped + 1 + needToSwitch + 1			
			),
			// We start with the initial state
			min(
				// We switch the lights individually, then we swap
				nr_switched + needToSwitch + 1,
				
				// We swap first, then switch the lights individually
				nr_switched + 1 + needToSwitchIfSwapped
			)
		);
		
		// Go through cases that start with initial state
		int new_nr_switched = min(
			// We start with the swapped state
			min(
				// We switch the individual lights, then swap
				nr_swapped + needToSwitchIfSwapped + 1,
				
				// We swap, then switch the individual lights
				nr_swapped + 1 + needToSwitch
			),
			// We start with the initial state
			min(
				// We switch the individual lights
				nr_switched + needToSwitch,
				
				// We swap, the switch individual lights, then swap back
				nr_switched + 1 + needToSwitchIfSwapped + 1
			)
		);
		
		nr_swapped = new_nr_swapped;
		nr_switched = new_nr_switched;
	}
	cout << min(nr_swapped, nr_switched) << endl;
}

int main() {
	cin.sync_with_stdio(false);
	cout.sync_with_stdio(false);
	
	int t; cin >> t;
	while(t--)
		testcase();
}