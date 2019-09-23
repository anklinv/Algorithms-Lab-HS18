#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <cassert>

using namespace std;

// Use global variables to have less on the stack and not run into a stack overflow due to the recursive calls
vector<vector<int>> children;
vector<int> path;
multiset<int> temps;
vector<int> temperature;
int difference;
int trip_length;
set<int> result;

// DFS from position pos
void DFS(const int &pos) {
	path.push_back(pos);
	temps.insert(temperature[pos]);
		
	if (temps.size() > trip_length) {
		// Get element to remove
		int rm = path.rbegin()[trip_length];
		
		// cout << "finding " << rm << " with temp: " << temperature[rm] << endl;
		auto it = temps.find(temperature[rm]);
		assert(it != temps.end());
		temps.erase(it);
	}
	if (temps.size() == trip_length) {
		// Check if safe
		int min = *temps.begin();
		int max = *temps.rbegin();
		if (max - min <= difference) {
			int start = path.rbegin()[trip_length-1];
			result.insert(start);
		}
	}
		
	for (int child : children[pos]) {
		DFS(child); 
	}
	
	// Go back in root direction
	path.pop_back();
	
	// Remove current temperature
	auto it = temps.find(temperature[pos]);
	assert(it != temps.end());
	temps.erase(it);
	
	// Check if necessary to add temperature that is trip_length away
	if (path.size() >= trip_length) {
		int i = path.rbegin()[trip_length-1];
		temps.insert(temperature[i]);
	}
}

void testcase() {
	int n;
	cin >> n >> trip_length >> difference;
	
	temperature = vector<int>(n);
	for (int i = 0; i < n; ++i) {
		cin >> temperature[i];
	}
	
	children = vector<vector<int>>(n);
	for (int i = 0; i < n-1; ++i) {
		int u, v;
		cin >> u >> v;
		children[u].push_back(v);
	}
	
	path.clear();
	path.reserve(n);
	temps.clear();
	result.clear();
	
	// Do a depth first search from the root while always keeping the full path (path) and a multiset of the latest trip_length many encountered temperatures. We use a multiset as it is always sorted and we can extract the minimum and maximum in O(1). 
	DFS(0);

	if (result.size() == 0) {
		cout << "Abort mission" << endl;
	} else {
		for (auto el : result) {
			cout << el << " ";
		}
		cout << endl;
	}
}

int main() {
	ios_base::sync_with_stdio(false);
	int t; cin >> t;
	while(t--)
		testcase();
}