#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <map>

using namespace std;

// int maxBoats(int minPos, int i, vector<pair<int, int> > &lengths, map<pair <int, int> , int> &memo) {
	// if (i == lengths.size()) {
		// return 0;
	// }
	// if (minPos > lengths[i].first) {
		// auto key = make_pair(minPos, i+1);
		// if (memo.find(key) == memo.end()) {
			// memo[key] = maxBoats(minPos, i+1, lengths, memo);
		// }
		// return memo[key];
	// }
	// else {
		// auto key1 = make_pair(minPos, i+1);
		// if (memo.find(key1) == memo.end()) {
			// memo[key1] = maxBoats(minPos, i+1, lengths, memo);
		// }
		// int dont = memo[key1];
		// auto key2 = make_pair(max(lengths[i].first, minPos + lengths[i].second), i+1);
		// if (memo.find(key2) == memo.end()) {
			// memo[key2] = maxBoats(max(lengths[i].first, minPos + lengths[i].second), i+1, lengths, memo) + 1;
			// // if (memo[key2] == 6) {
				// // cout << "i= " << i << ", minPos= " << minPos << endl;
			// // }
		// }
		// int take = memo[key2];
		// if (dont >= take) {
			// return dont;
		// }
		// else {
			// return take;
		// }
	// }
// }

void testcase() {
	int n; cin >> n;
	vector<pair<int, int> > lengths(n);
	for (int i = 0; i < n; i++) {
		int l, p;
		cin >> l >> p;
		lengths[i] = make_pair(p, l);
	}
	
	sort(lengths.begin(), lengths.end());
	
	// map<pair <int, int> , int> memo;
	
	int minPos = numeric_limits<int>::min();
	int minPosBefore = numeric_limits<int>::min();
	// cout << maxBoats(minPos, 0, lengths, memo) << endl;
	int count = 0;
	for (int i = 0; i < n; ++i) {
		if (minPos > lengths[i].first) {
			minPos = min(minPos, max(lengths[i].first, minPosBefore + lengths[i].second));
			continue;
		} else {
			count++;
			minPosBefore = minPos;
			minPos = max(lengths[i].first, minPos + lengths[i].second);
		}
	}
	cout << count << endl;
	
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
	while(t--)
		testcase();
}
