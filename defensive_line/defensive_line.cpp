#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Recursively find the maximal number of intervals of sum k that we can have starting at 0 and ending at n-1.
int subproblem(const int &from, const int &to, const vector<int> &v, const int &k, const int &m, vector<vector<int>> &memo, vector<int> &choices) {

	// If there is no attacker left, we can not have an interval
	if (m == 0) {
		return 0;
	}

	// Hacky way to express that this is not possible.
	if (from >= to) {
		return -1000000;
	}
	
	// Check if there is an interval starting at this position
	if (choices[from] != -1) {
		
		// The remaining interval starts at position i
		int i = choices[from] + 1;
		
		if (memo[from + 1][m] == -1) {
			
			// We do not take the interval
			memo[from + 1][m] = subproblem(from + 1, to, v, k, m, memo, choices);

		}
		if (memo[i][m - 1] == -1) {
			
			// We take the interval
			memo[i][m - 1] = subproblem(i, to, v, k, m - 1, memo, choices);

		}
		
		// We take the better of both choices
		return max(memo[from + 1][m], (i - from) + memo[i][m - 1]);
	}
	// There is no interval starting at this position
	else {
		// If we did not compute it already, compute now.
		if (memo[from + 1][m] == -1) {
			memo[from + 1][m] = subproblem(from + 1, to, v, k, m, memo, choices);
		}
		return memo[from + 1][m];
	}

}

void testcase() {
	int n, m, k;
	cin >> n; cin >> m; cin >> k;
	vector<int> v(n);
	
	// For each starting point with m many attackers save the result.
	vector<vector<int>> memo(n + 1, vector<int>(m + 1, -1));
	
	for (int i = 0; i < n; i++) {
		cin >> v[i];
	}

	// Precompute the choices:
	// Since all attackers have the same strengths and since we can only attack if there exists an interval of defenders such that their sum of defensive value is equal to the k (the attack value), we can precompute for every possible start of the interval:
	// If it is impossible to start an interval there (value -1) or if it is possible, at which index the interval ends. We compute this using sliding window.
	vector<int> choices(n, -1);
	int j = 0;
	int sum = v[0];
	for (int i = 0; i < n; i++) {
		if (sum < k) {
			j++;
			if (j >= n) {
				break;
			}
			sum += v[j];
			i--;
		}
		else if (sum == k) {
			choices[i] = j;
			sum -= v[i];
		}
		else { // sum > k
			sum -= v[i];
		}
	}

	// Recursively find the maximal number of intervals of sum k that we can have starting at 0 and ending at n-1.
	int maxValue = subproblem(0, n - 1, v, k, m, memo, choices);

	if (maxValue < 0) {
		cout << "fail" << endl;
	}
	else {
		cout << maxValue << endl;
	}
}

int main() {
	int t; cin >> t;
	while (t--) {
		testcase();
	}
}