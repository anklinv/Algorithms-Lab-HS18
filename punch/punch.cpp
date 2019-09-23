#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <climits>
#include <stdexcept>

using namespace std;

struct Beverage {
	int cost;
	int volume;
};

struct Punch {
	int cost;
	int variety;
};

void testcase() {
	int nr_beverages, total_amount; cin >> nr_beverages >> total_amount;
	vector<Beverage> beverages(nr_beverages);
	int maxVol = 0;
	for (int i = 0; i < nr_beverages; ++i) {
		int cost, volume;
		cin >> cost >> volume;
		beverages[i] = {cost, volume};
		maxVol = max(maxVol, volume);
	}
	
	maxVol = maxVol + total_amount - 1;
	
	// for (auto b : beverages) {
		// cout << "beverage " << b.cost << " " << b.volume << endl;
	// }
	
	// DP Table
	vector<vector<Punch>> dp(nr_beverages, vector<Punch>(maxVol));
	// vector<vector<Punch>> dp(nr_beverages, vector<Punch>(total_amount+1));
	
	dp[0][0] = {0, 0};
	dp[0][1] = {beverages[0].cost, 1};
	// Initialize top row
	for (int i = 2; i < maxVol; ++i) {
	// for (int i = 2; i <= total_amount; ++i) {
		dp[0][i] = {((beverages[0].volume + i - 1) /beverages[0].volume)*beverages[0].cost, 1};
	}
	// Initialize left column
	for (int i = 1; i < nr_beverages; ++i) {
		dp[i][0] = {0, 0};/*
		if (dp[i-1][1].cost < beverages[i].cost) {
			dp[i][1] = dp[i-1][1];
		} else {
			dp[i][1] = {beverages[i].cost, 1};
		}*/
	}
	
	// cout << "INITIALIZED" << endl;
	// for (int n = 0; n < nr_beverages; ++n) {
		// for (int k = 0; k <= total_amount; ++k) {
		// cout << "{" << dp[n][k].cost << " " << dp[n][k].variety << "}" << " ";
		// }
		// cout << endl;
	// }
	
	for (int n = 1; n < nr_beverages; ++n) {
		for (int k = 1; k < maxVol; ++k) {
		// for (int k = 2; k <= total_amount; ++k) {
			Punch dontTakeIt = dp[n-1][k];
			Punch takeIt;
			int k_take = k - beverages[n].volume;
			if (k_take <= 0) {
				// if we take it we dont need more
				takeIt = {beverages[n].cost, 1};
			} else {
				// we still have some remaining capacity
				if (dp[n-1][k_take].variety == dp[n][k_take].variety && dp[n-1][k_take].cost == dp[n][k_take].cost) {
					// variety increases as we did not take it before
					takeIt = {beverages[n].cost + dp[n][k_take].cost, dp[n][k_take].variety + 1};
					// cout << "increasing variety from " << dp[n][k_take].variety << " to " << dp[n][k_take].variety + 1 << endl;
				} else {
					// variety does not increase
					takeIt = {beverages[n].cost + dp[n][k_take].cost, dp[n][k_take].variety};
				}
			}
			// Take if cheaper or same to keep (variety always increases or stays the same)
			if ((takeIt.cost < dontTakeIt.cost) || ((takeIt.cost == dontTakeIt.cost) && (takeIt.variety >= dontTakeIt.variety))) {
				// if ((takeIt.cost == dontTakeIt.cost) && (takeIt.variety > dontTakeIt.variety)) {
					// cout << "take it since " << takeIt.cost << "<=" << dontTakeIt.cost << " and " << takeIt.variety << " " << dontTakeIt.variety << endl;
				// }
				// cout << "take it since " << takeIt.cost << "<=" << dontTakeIt.cost << " and " << takeIt.variety << " " << dontTakeIt.variety << endl;
				dp[n][k] = takeIt;
			} else {
				// cout << "dont take it since " << takeIt.cost << "<=" << dontTakeIt.cost << " and " << takeIt.variety << " " << dontTakeIt.variety << endl;
				dp[n][k] = dontTakeIt;
			}
		}
	}
	
	// cout << "FINISHED COMPUTING" << endl;
	// for (int n = 0; n < nr_beverages; ++n) {
		// for (int k = 0; k < maxVol; ++k) {
		// cout << "{" << dp[n][k].cost << " " << dp[n][k].variety << "}" << " ";
		// }
		// cout << endl;
	// }
	
	cout << dp[nr_beverages-1][total_amount].cost << " " << dp[nr_beverages-1][total_amount].variety << endl;
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
	while(t--) {
		testcase();
	}
}
