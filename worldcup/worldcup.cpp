// example: decide whether there exists a disk that covers a given set
// of points R in the plane and is disjoint from another set of points B
#include <iostream>
#include <cassert>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <set>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

// choose exact integral type
#include <CGAL/Gmpq.h>
typedef CGAL::Gmpq ET;

// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Vertex_handle Vh;

typedef K::Point_2 Point;

struct Warehouse {
	Point p;
	int supply;
	int alc;
	int i;
};

struct Stadium {
	Point p;
	int demand;
	int limit;
	int i;
};

using namespace std;

double floor_to_double(CGAL::Quotient<ET> x) {
	double a = floor(CGAL::to_double(x));
	while(a>x) a-=1;
	while(a+1 <= x) a+=1;
	return a;
}

void testcase() {
	cout << fixed << setprecision(0);
	
	int nr_warehouses, nr_stadiums, nr_contours;
	cin >> nr_warehouses >> nr_stadiums >> nr_contours;
	
	vector<Point> relevant;
	
	vector<Warehouse> warehouses;
	for (int i = 0; i < nr_warehouses; ++i) {
		int x, y, s, a;
		cin >> x >> y >> s >> a;
		warehouses.push_back({Point(x, y), s, a, i});
		relevant.push_back(Point(x, y));
	}
	
	vector<Stadium> stadiums;
	for (int i = 0; i < nr_stadiums; ++i) {
		int x, y, d, u;
		cin >> x >> y >> d >> u;
		stadiums.push_back({Point(x, y), d, u, i});
		relevant.push_back(Point(x, y));
	}
	
	vector<vector<ET> > revenues(nr_warehouses, vector<ET>(nr_stadiums));
	for (int i = 0; i < nr_warehouses; ++i) {
		for (int j = 0; j < nr_stadiums; ++j) {
			cin >> revenues[i][j];
		}
	}
	
	// Triangulate everything that is relevant
	Triangulation t;
	t.insert(relevant.begin(), relevant.end());
	
	// Find all contours that are relevant
	vector<pair<Point, K::FT> > contourLines;
	for (int i = 0; i < nr_contours; ++i) {
		int x, y, r;
		cin >> x >> y >> r;
		
		Vh v = t.nearest_vertex(Point(x, y));
		if (CGAL::squared_distance(v->point(), Point(x, y)) <= K::FT(r) * K::FT(r)) {
			contourLines.push_back(make_pair(Point(x, y), K::FT(r)));
		}
	}
	
	assert(contourLines.size() <= 100);
	
	vector<set<int> > warehouseSet(nr_warehouses);
	vector<set<int> > stadiumSet(nr_stadiums);
	int contourIndex = 0;
	for (auto c : contourLines) {
		for (auto w : warehouses) {
			if (CGAL::squared_distance(c.first, w.p) <= c.second * c.second) {
				warehouseSet[w.i].insert(contourIndex);
			}
		}
		for (auto s : stadiums) {
			if (CGAL::squared_distance(c.first, s.p) <= c.second * c.second) {
				stadiumSet[s.i].insert(contourIndex);
			}
		}
		contourIndex++;
	}
	
	// Go through all pairs and adjust revenues
	for (auto w : warehouses) {
		for (auto s : stadiums) {
			set<int> diff1;
			set<int> diff2;
			set_difference(warehouseSet[w.i].begin(), warehouseSet[w.i].end(), stadiumSet[s.i].begin(), stadiumSet[s.i].end(), inserter(diff1, diff1.begin()));
			set_difference(stadiumSet[s.i].begin(), stadiumSet[s.i].end(), warehouseSet[w.i].begin(), warehouseSet[w.i].end(),inserter(diff2, diff2.begin()));
			int nr_crossings = diff1.size() + diff2.size();
			if (nr_crossings != 0) {
				// cout << setprecision(3);
				// cout << "revenue change from " << CGAL::to_double(revenues[w.i][s.i]) << " to ";
				revenues[w.i][s.i] = revenues[w.i][s.i] - (ET(nr_crossings) / ET(100));
				// cout << CGAL::to_double(revenues[w.i][s.i]) << endl;
			}
		}
	}
	
	Program lp (CGAL::SMALLER, true, 0, false, 0); 
	
	int constraintCount = 0;
	
	// Constraints on supply
	for (auto w : warehouses) {
		for (auto s : stadiums) {
			lp.set_a(w.i * nr_stadiums + s.i, constraintCount, 1);
		}
		lp.set_b(constraintCount, w.supply);
		constraintCount++;
	}
	
	// Contraints on demand met
	for (auto s : stadiums) {
		for (auto w : warehouses) {
			lp.set_a(w.i * nr_stadiums + s.i, constraintCount, 1);
			lp.set_a(w.i * nr_stadiums + s.i, constraintCount+1, -1);
		}
		lp.set_b(constraintCount, s.demand);
		lp.set_b(constraintCount+1, -s.demand);
		constraintCount += 2;
	}
	
	// Constraints on alcohol limit
	for (auto s : stadiums) {
		for (auto w : warehouses) {
			lp.set_a(w.i * nr_stadiums + s.i, constraintCount, w.alc);
		}
		lp.set_b(constraintCount, 100 * s.limit);
		constraintCount++;
	}
	
	// Set objective
	for (auto w : warehouses) {
		for (auto s : stadiums) {
			lp.set_c(w.i * nr_stadiums + s.i, -revenues[w.i][s.i]);
		}
	}
	
	Solution s = CGAL::solve_nonnegative_linear_program(lp, ET());
	assert (s.solves_linear_program(lp));
	
	if (s.is_infeasible()) {
		cout << "RIOT!" << endl;
	} else {
		assert(s.is_optimal());
		cout << floor_to_double(-s.objective_value()) << endl;
	}
}

int main() {
	int t; cin >> t;
	while(t--)
		testcase();
}
