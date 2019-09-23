#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int maxMoves(int currentPosition, vector<vector<int>> &transitions, int moves, vector<int> &memoMin, vector<int> &memoMax);

int minMoves(int currentPosition, vector<vector<int>> &transitions, int moves, vector<int> &memoMin, vector<int> &memoMax) {
	int n = transitions.size() - 1;
	if (currentPosition == n) {
		return 0;
	}
	int minMove = numeric_limits<int>::max();
	for (auto nextPosition : transitions[currentPosition]) {
		if (memoMin[nextPosition] == -1) {
			memoMin[nextPosition] = maxMoves(nextPosition, transitions, moves, memoMin, memoMax);
		}
		minMove = min(minMove, memoMin[nextPosition]);
	}
	return moves + 1 + minMove;
}

int maxMoves(int currentPosition, vector<vector<int>> &transitions, int moves, vector<int> &memoMin, vector<int> &memoMax) {
	int n = transitions.size() - 1;
	if (currentPosition == n) {
		return 0;
	}
	int maxMove = 0;
	for (auto nextPosition : transitions[currentPosition]) {
		if (memoMax[nextPosition] == -1) {
			memoMax[nextPosition] = minMoves(nextPosition, transitions, moves, memoMin, memoMax);
		}
		maxMove = max(maxMove, memoMax[nextPosition]);
	}
	return moves + 1 + maxMove;
}

void testcase() {
	int n, m;
	cin >> n; cin >> m;
	int r, b;
	cin >> r; cin >> b;
	vector<vector<int>> transitions(n+1);

	vector<int> memoMin(n + 1, -1);
	vector<int> memoMax(n + 1, -1);

	for (int i = 0; i < m; i++) {
		int from, to;
		cin >> from; cin >> to;
		transitions[from].push_back(to);
	}

	int sherlockMoves = minMoves(r, transitions, 0, memoMin, memoMax);
	int moriartyMoves = minMoves(b, transitions, 0, memoMin, memoMax);

	if (sherlockMoves < moriartyMoves)
		cout << 0 << endl;
	else if (sherlockMoves == moriartyMoves && (sherlockMoves % 2 == 0))
		cout << 1 << endl;
	else if (sherlockMoves == moriartyMoves)
		cout << 0 << endl;
	else
		cout << 1 << endl;
}

int main() {
	int t; cin >> t;
	while (t--) {
		testcase();
	}
}