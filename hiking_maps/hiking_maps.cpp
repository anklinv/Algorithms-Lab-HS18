#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

struct Triangle {
	K::Point_2 q0, q1, q2, q3, q4, q5;
	Triangle(K::Point_2 _q0, K::Point_2 _q1, K::Point_2 _q2, K::Point_2 _q3, K::Point_2 _q4, K::Point_2 _q5) {
		q0 = _q0;
		q1 = _q1;
		q2 = _q2;
		q3 = _q3;
		q4 = _q4;
		q5 = _q5;
	}
	
	bool contains(K::Point_2 &p) {
	// All are to the right --> must be inside
	return !CGAL::left_turn(this->q0, this->q1, p)
	    && !CGAL::left_turn(this->q2, this->q3, p)
		&& !CGAL::left_turn(this->q4, this->q5, p);
	}
};

void testcase() {
	int m, n;
	cin >> m >> n;
	
	vector<K::Point_2> path;
	for (int i = 0; i < m; i++) {
		int x,y;
		cin >> x >> y;
		path.push_back(K::Point_2(x, y));
	}
	
	vector<Triangle> t;
	for (int i = 0; i < n; i++) {
		vector<K::Point_2> q;
		for (int j = 0; j < 6; j++) {
			int x, y;
			cin >> x >> y;
			q.push_back(K::Point_2(x, y));
		}
		
		// Ensure same orientation
		if (CGAL::left_turn(q[0], q[1], q[2]))
			swap(q[0], q[1]);
		if (CGAL::left_turn(q[2], q[3], q[4]))
			swap(q[2], q[3]);
		if (CGAL::left_turn(q[4], q[5], q[0]))
			swap(q[4], q[5]);
		
		t.push_back(Triangle(q[0], q[1], q[2], q[3], q[4], q[5]));
	}
	
	vector< vector<int> > triangleSets(n);
	
	// Compute which segments are in each triangle
	int triangleIndex = 0;
	for (auto triangle : t) {
		for (int i = 0; i < m - 1; i++) {
			if (triangle.contains(path[i]) && triangle.contains(path[i+1])) {
				triangleSets[triangleIndex].push_back(i);
			}
		}
		triangleIndex++;
	}
	
	bool allCovered = false;
	int minCost = 2000;
	int b = 0; int e = -1;
	vector<int> count(m-1, 0);
	
	while (b < n) {
		if (allCovered) {
			for (auto el : triangleSets[b]) {
				count[el]--;
			}
			b++;
		} else {
			if (e == n - 1) {
				break;
			}
			e++;
			for (auto el : triangleSets[e]) {
				count[el]++;
			}
		}
			
		for (int i = 0; i < m; i++) {
			if (i == m - 1) {
				allCovered = true;
				minCost = min(minCost, e - b + 1);
				break;
			} else {
				if (count[i] < 1) {
					allCovered = false;
					break;
				}
			}
		}
	}
	
	cout << minCost << endl;
}

int main() {
	int t; cin >> t;
	while(t--) {
		testcase();
	}
}