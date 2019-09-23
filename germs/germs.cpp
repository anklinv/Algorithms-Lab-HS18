#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <climits>
#include <map>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Point Point;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Vertex_handle Vertex_handle;
typedef Triangulation::Face_handle Face_handle;
typedef Triangulation::Finite_vertices_iterator FVertexIt;
typedef Triangulation::Finite_edges_iterator FEdgeIt;

using namespace std;

double ceil_to_double(const K::FT& x) {
	double a = std::ceil(CGAL::to_double(x));
	while (a < x) a += 1;
	while (a-1 >= x) a -= 1;
	return a;
}

int main() {
	cin.sync_with_stdio(false);
    cout.sync_with_stdio(false);
	cout << fixed << setprecision(0);

	while (true) {
		int n; cin >> n;
		if (n == 0) {
			break;
		}
		
		int l, b, r, t; cin >> l >> b >> r >> t;
		
		vector<Point> pts;
		pts.reserve(n);
		for (size_t i = 0; i < n; ++i) {
			int x, y;
			cin >> x >> y;
			pts.push_back(Point(x, y));
		}
		
		// construct triangulation
		Triangulation tri;
		tri.insert(pts.begin(), pts.end());
		
		map<Point, double> distances; 
		
		// Find closest point to bounding box
		for (FVertexIt v = tri.finite_vertices_begin(); v != tri.finite_vertices_end(); ++v) {
			K::FT box = v->point().x() - l;
			box = min(box, r - v->point().x());
			box = min(box, v->point().y() - b);
			box = min(box, t - v->point().y());
			distances[v->point()] = CGAL::to_double(box * box);
		}
		
		// Find closest point to others
		for (FEdgeIt e = tri.finite_edges_begin(); e != tri.finite_edges_end(); ++e) {
			Vertex_handle v1 = e->first->vertex((e->second + 1) % 3);
			Vertex_handle v2 = e->first->vertex((e->second + 2) % 3);
			double distance = CGAL::to_double(CGAL::squared_distance(v1->point(), v2->point()) / 4);
			distances[v1->point()] = min(distances[v1->point()], distance);
			distances[v2->point()] = min(distances[v2->point()], distance);
		}
		
		// Put in vector
		vector<double> sortedDistances;
		sortedDistances.reserve(n);
		for (auto pair : distances)
			sortedDistances.push_back(pair.second);
		
		// Sort vector
		sort(sortedDistances.begin(), sortedDistances.end());
		
		auto deathTime = [] (const double &distance) {
			return max(0.0, ceil(sqrt(sqrt(distance) - 0.5)));
		};
		
		int len = sortedDistances.size();
		
		cout << deathTime(sortedDistances[0]) << " " << deathTime(sortedDistances[len/2]) << " " << deathTime(sortedDistances[len-1]) << endl;
	}
}