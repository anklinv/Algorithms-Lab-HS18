#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <map>

using namespace std;

struct Seg {
	int start, end;
	Seg(int s, int e) : start(s), end(e) {};
};

void testcase() {
	int n,m;
	cin >> n >> m;

	vector<Seg> allSegs;
	map<int, int> counters;
	
	for (int i = 0; i < n; ++i) {
		int a, b;
		cin >> a >> b;
		a--; b--; // zero indexed
		allSegs.push_back(Seg(a, b));
		counters[a]++;
		counters[b]--;
		if (a > b)
			counters[0]++;
	}
	
	// Find segment with at most 10 overlaps
	int start = 0;
	int min_overlaps = 11;
	int overlaps = 0;
	for (auto c : counters) {
		overlaps += c.second;
		if (overlaps < min_overlaps) {
			start = c.first;
			min_overlaps = overlaps;
		}
	}
	
	// Shift stuff around by "start"
	vector<Seg> normal, wrapAround;
	for (auto seg : allSegs) {
		int newStart;
		if (seg.start - start < 0) {
			newStart = seg.start - start + m;
		} else {
			newStart = seg.start - start;
		}
		int newEnd;
		if (seg.end - start < 0) {
			newEnd = seg.end - start + m;
		} else {
			newEnd = seg.end - start;
		}
		if (newStart <= newEnd) {
			normal.push_back(Seg(newStart, newEnd));
		} else {
			wrapAround.push_back(Seg(newStart, newEnd));
		}
	}
	
	// Sort according to end time
	sort(normal.begin(), normal.end(), [] (Seg &p, Seg &q) { return (p.end < q.end) || (p.end == q.end && p.start > q.start); } );
	
	auto findCount = [&normal] (int startIndex, int endIndex) {
		int count = 0;
		int right = startIndex;
		for (auto seg : normal) {
			if (seg.start > right && seg.end < endIndex) {
				count++;
				right = seg.end;
			}
		}
		return count;
	};
	
	int maxCount = findCount(0, m);
	for (auto wrap : wrapAround) {
		maxCount = max(maxCount, 1 + findCount(wrap.end, wrap.start));
	}
	
	cout << maxCount << endl;
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
	while(t--)
		testcase();
}
