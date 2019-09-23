#include <iostream>
#include <cassert>
#include <vector>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>

// choose exact integral type
#include <CGAL/Gmpzf.h>
typedef CGAL::Gmpzf ET;

// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;

struct Cell {
	int x, y, z, i;
};

bool possible(const int degree, const vector<Cell> &healthy, const vector<Cell> &tumor) {

	Program lp (CGAL::SMALLER, false, 0, false, 0); 
	
	int epsilon = 0;
	int coefficientIndex = 1;
	for (int degx = 0; degx <= degree; ++degx) {
		for (int degy = 0; degy <= degree - degx; ++degy) {
			for (int degz = 0; degz <= degree - degx - degy; ++degz) {
				
				for (Cell h : healthy) {
					double value = pow(h.x, degx) * pow(h.y, degy) * pow(h.z, degz);
					lp.set_a(coefficientIndex, h.i, value);
				}
				for (Cell t : tumor) {
					double value = pow(t.x, degx) * pow(t.y, degy) * pow(t.z, degz);
					lp.set_a(coefficientIndex, t.i, value);
				}
				coefficientIndex++;
			}
		}
	}
	
	// Add strictly smaller/larger term
	for (Cell h : healthy) {
		lp.set_a(epsilon, h.i, 1.0);
		lp.set_b(h.i, 0.0);
	}
	for (Cell t : tumor) {
		lp.set_a(epsilon, t.i, -1.0);
		lp.set_b(t.i, 0.0);
		lp.set_r(t.i, CGAL::LARGER);
	}
	
	// maximize epsilon
	lp.set_c(epsilon, -1.0);
	lp.set_l(epsilon, true, 0.0);
	lp.set_u(epsilon, true, 1.0);
	
	CGAL::Quadratic_program_options options;
	options.set_pricing_strategy(CGAL::QP_BLAND);
	Solution s = CGAL::solve_linear_program(lp, ET(), options);
	assert (s.solves_linear_program(lp));
	// cout << s << endl;
	return !s.is_infeasible() && (s.objective_value() != 0.0);
}

void testcase() {
	int h, t;
	cin >> h >> t;
	
	vector<Cell> healthy;
	for (int i = 0; i < h; ++i) {
		int x, y, z;
		cin >> x >> y >> z;
		healthy.push_back({x, y, z, i});
	}
	
	vector<Cell> tumor;
	for (int i = 0; i < t; ++i) {
		int x, y, z;
		cin >> x >> y >> z;
		tumor.push_back({x, y, z, i + h});
	}
	
	int l = 0;
	int r = 30;
	while (l < r) {
		int m = (l+r)/2;
		if (possible(m, healthy, tumor)) {
			r = m;
		} else {
			l = m+1;
		}
	}
	assert(l == r);
	
	if (l != 30) {
		cout << l << endl;
	} else {
		cout << "Impossible!" << endl;
	}
}

int main() {
	int T; cin >> T;
	while(T--)
		testcase();
}
