#include <iostream>
#include <vector>
#include <set>
#include <cassert>

using namespace std;

long long moves(int from, int moves_left, const vector<set<int>> &connections, const vector<vector<long long>> &points, vector<vector<long long>> &memo) {
	if (memo[from][moves_left] == -1) {
		// cout << "calculating from " << from << " with " << moves_left << endl;
		long long maxPoints = 0;
		for (auto next : connections[from]) {
			maxPoints = max(maxPoints, 
							points[from][next] + moves(next, moves_left - 1, connections, points, memo));
		}
		assert(maxPoints >= 0);
		memo[from][moves_left] = maxPoints;
	}
	// cout << "returning " << memo[from][moves_left] << endl;
	assert(memo[from][moves_left] >= 0);
	return memo[from][moves_left];
}

void testcase(int t) {
	// cout << "testcase" << endl;
	int nr_holes, nr_canals, nr_moves; long long target_score;
	cin >> nr_holes >> nr_canals >> target_score >> nr_moves;
	// if (t==0) {
		// cout << "TESTCASE" << endl;
		// cout << "nr_holes " << nr_holes << endl;
		// cout << "nr_canals " << nr_canals << endl;
		// cout << "target_score " << target_score << endl;
		// cout << "nr_moves " << nr_moves << endl;
	// }
	
	vector<set<int>> connections(nr_holes);
	vector<vector<long long>> points(nr_holes, vector<long long>(nr_holes, 0));
	for (int i = 0; i < nr_canals; ++i) {
		int u, v; long long p;
		cin >> u >> v >> p;
		connections[u].insert(v);
		points[u][v] = max(points[u][v], p);
		// assert(points[u][v] > 0);
	}
	
	// Find synonyms for 0
	vector<bool> end(nr_holes, false);
	for (int i = 0; i < nr_holes; ++i) {
		if (connections[i].size() == 0) end[i] = true;
	}
	
	vector<set<int>> newConnections(nr_holes);
	vector<vector<long long>> newPoints(nr_holes, vector<long long>(nr_holes, 0));
	for (int i = 0; i < nr_holes; ++i) {
		for (auto el : connections[i]) {
			if (end[el]) {
				newConnections[i].insert(0);
				newPoints[i][0] = max(newPoints[i][0], points[i][el]);
			} else {
				newConnections[i].insert(el);
				newPoints[i][el] = points[i][el];
			}
		}
	}
	
	// cout << "connections" << endl;
	// for (int i = 0; i < nr_holes; ++i) {
		// cout << i << " is connected to ";
		// for (auto el : newConnections[i]) {
			// cout << el << " ";
		// }
		// cout << endl;
	// }
	
	// cout << "read in successful" << endl;
	vector<vector<long long>> memo(nr_holes, vector<long long>(nr_moves + 1, -1));
	
	for (auto row : memo) {
		for (auto el : row) {
			assert(el == -1);
		}
	}
	
	for (int i = 0; i < nr_holes; ++i) {
		memo[i][0] = 0;
	}
	
	for (int i = 0; i <= nr_moves; ++i) {
		long long possible = moves(0, i, newConnections, newPoints, memo);
		if (possible >= target_score) {
			cout << i << endl;
			return;
		}
	}
	// if (t == 0) {
		// cout << "maxel" << endl;
		// long long maxEl = 0;
		// for (auto row : memo) {
			// for (auto el : row) {
				// maxEl = max(maxEl, el);
			// }
		// }
		// cout << "max score " << maxEl << " target: " << target_score << endl;
	// }
	// if (t == 0) {
		// for (int i = 0; i < nr_holes; ++i) {
			// for (int j = 0; j <= nr_moves; ++j){
				// if (memo[i][j] < -1) {
					// cout << memo[i][j] << endl;
				// }
			// }
		// }
	// }
	cout << "Impossible" << endl;
	
	// for (auto row : memo) {
		// for (auto el : row) {
			// cout << el << " ";
		// }
		// cout << endl;
	// }
}

int main() {
	ios_base::sync_with_stdio(false);
	int t; cin >> t;
	while(t--) {
		testcase(t);
		// break;
	}
}