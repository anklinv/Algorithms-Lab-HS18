#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <limits>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Vertex_handle Vertex_handle;

using namespace std;

int main() {
	cin.sync_with_stdio(false);
    cout.sync_with_stdio(false);
	cout << fixed << setprecision(0);

	while (true) {
		int n; cin >> n;
		// cout << "n = " << n << endl;
		if (n == 0) {
			break;
		}
		
		vector<K::Point_2> pts;
		pts.reserve(n);
		for (size_t i = 0; i < n; ++i) {
			int x, y;
			cin >> x >> y;
			pts.push_back(K::Point_2(x, y));
		}
		
		// construct triangulation
		Triangulation t;
		t.insert(pts.begin(), pts.end());
		
		int m; cin >> m;
		for (int i = 0; i < m; ++i) {
			// Read in new vertex
			int x, y;
			cin >> x >> y;
			
			K::Point_2 location = K::Point_2(x, y);
			K::Point_2 nearest = t.nearest_vertex(location)->point();
			
			cout << CGAL::squared_distance(location, nearest) << endl;
			
			// // Insert vertex
			// Vertex_handle v = t.insert(K::Point_2(x, y));
			
			// // Go over all incident edges
			// Triangulation::Edge_circulator c = t.incident_edges(v);
			// double squaredDistance = numeric_limits<double>::infinity();
			// do {
				// if (!t.is_infinite(c)) {
					// Triangulation::Vertex_handle v1 = c->first->vertex((c->second + 1) % 3);
					// Triangulation::Vertex_handle v2 = c->first->vertex((c->second + 2) % 3);
					// squaredDistance = min(squaredDistance, CGAL::squared_distance(v1->point(), v2->point()));
				// }
			// } while (++c != t.incident_edges(v));
			// cout << squaredDistance << endl;
			
			// // Remove vertex
			// t.remove(v);
		}
	}
}