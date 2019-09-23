#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <iostream>
#include <vector>
#include <algorithm>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef K::Point_2 Point;

using namespace std;

void testcase() {
	int m, n;
	cin >> m >> n;
	
	vector<pair<Point, int> > participants;
	participants.reserve(m);
	for (int i = 0; i < m; ++i) {
		int x, y, r;
		cin >> x >> y >> r;
		participants.push_back(make_pair(Point(x, y), r));
	}
	
	int h; cin >> h;
	
	vector<Point> lights;
	lights.reserve(n);
	for (int i = 0; i < n; ++i) {
		int x, y;
		cin >> x >> y;
		lights.push_back(Point(x, y));
	}
	
	// construct triangulation
	Triangulation t;
	t.insert(lights.begin(), lights.end());
	
	// Go over all participants;
	vector<int> winners;
	vector<int> firstHit(m, -1);
	int maxFirstHit = 0;
	for (int i = 0; i < m; ++i) {
		auto nearest = t.nearest_vertex(participants[i].first);
		if (CGAL::squared_distance(nearest->point(), participants[i].first) >= pow(participants[i].second + h, 2)) {
			winners.push_back(i);
			continue;
		} else {
			for (int j = 0; j < n; ++j) {
				if (CGAL::squared_distance(lights[j], participants[i].first) < pow(participants[i].second + h, 2)) {
					firstHit[i] = j;
					maxFirstHit = max(maxFirstHit, j);
					break;
				}
			}
		}
	}
	
	if (winners.size() == 0) {
		for (int i = 0; i < m; ++i) {
			if (firstHit[i] == maxFirstHit) {
				winners.push_back(i);
			}
		}
	}
	
	sort(winners.begin(), winners.end());

	for (auto winner : winners) {
		cout << winner << " ";
	}
	cout << endl;
}

int main()
{
	int t; cin >> t;
	while(t--)
		testcase();
}