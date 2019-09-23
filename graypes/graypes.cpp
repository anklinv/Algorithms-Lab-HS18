#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

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
		
		K::FT minLength;
		bool fst = true;
		// output all edges
		for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
			K::FT l = t.segment(e).squared_length();
			if (fst || minLength > l) {
				minLength = l;
				fst = false;
			}
		}
		// cout << "minLength = " << ceil_to_double(minLength) << endl;
		double x = ceil(50 * std::sqrt(CGAL::to_double(minLength)));
		cout << x << endl;
	}
}