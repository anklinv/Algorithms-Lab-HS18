#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;

// Recursive function that computes the probability to get to get our current wealth k over m in i days.
double probFunction(const vector<double> &p, const int &m, const int &n, int i, int k, vector<vector<double>> &mem) {
	
	// We already managed to get more wealth than we need -> probability 1
	if (k >= m) {
		return 1.0;
	}
	
	// We ran out of days, we will never reach the goal -> probability 0
	if (i >= n) {
		return 0.0;
	}
	
	// At the last day, we can win need less money than we have currently.
	if (i == n - 1) {
		int bet = m - k;
		
		// We would need more than we can win in a single round
		if (bet > k)
			return 0.0;
		
		// We can win enough in a single round -> probability depends on the winning probability
		else
			return p[i];
	}
	
	// If we have no money to bet, we can never make money -> probability that we make it is 0
	if (k == 0) {
		return 0.0;
	}
	
	// Go over all possible bet amounts
	double maxProb = 0;
	for (int bet = 0; bet <= k; bet++) {
		
		// Ignore bets that would get us more money than we need (they all stop in the next iteration).
		if (k + bet > m) {
			continue;
		}
		
		double winProb, looseProb;
		
		// Case 1: We actually win the bet
		if (mem[k + bet][i + 1] != -1) {
			winProb = mem[k + bet][i + 1];
		}
		else {
			// We now have bet more on the next day
			winProb = probFunction(p, m, n, i + 1, k + bet, mem);
			mem[k + bet][i + 1] = winProb;
		}
		
		// Case 2: We loose the bet
		if (mem[k - bet][i + 1] != -1) {
			looseProb = mem[k - bet][i + 1];
		}
		else {
			// We now have bet less on the next day
			looseProb = probFunction(p, m, n, i + 1, k - bet, mem);
			mem[k - bet][i + 1] = looseProb;
		}
		
		// Compute the probability that we make it in total
		double newProb = p[i] * winProb + (1 - p[i]) * looseProb;
		maxProb = max(maxProb, newProb);
	}
	
	// Return the maximum of all possible bets
	return maxProb;
}

void testcase() {
	int n, k, m;
	cin >> n; cin >> k; cin >> m;
	vector<double> p;
	for (int i = 0; i < n; i++) {
		double tmp;
		cin >> tmp;
		p.push_back(tmp);
	}

	// Memorization for all subproblems with (i,j) meaning the probability to get to wealth i after j days.
	vector<vector<double>> mem(m + 1, vector<double>(n, -1));

	cout << std::fixed << std::setprecision(5);
	
	// Check if we can get to wealth m in n days
	cout << probFunction(p, m, n, 0, k, mem) << endl;
}

int main() {
	int t; cin >> t;
	for (int i = 0; i < t; i++) {
		testcase();
	}
}