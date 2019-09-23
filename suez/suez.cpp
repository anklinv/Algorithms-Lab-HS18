#include <iostream>
#include <cassert>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <vector>
#include <climits>

// choose exact integral type
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<long> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
typedef CGAL::Quotient<ET> SolT;

using namespace std;

double ceil_to_double(const SolT& x) {
	double a = std::ceil(CGAL::to_double(x));
	while (a < x) a += 1;
	while (a-1 >= x) a -= 1;
	return a;
}

void testcase() {
	// Read input
	int n, m, h, w;
	cin >> n >> m >> h >> w;
	
	vector<long> newX(n);
	vector<long> newY(n);
	for (int i = 0; i < n; ++i) {
		cin >> newX[i] >> newY[i];
	}
	
	vector<long> oldX(m);
	vector<long> oldY(m);
	for (int i = 0; i < m; ++i) {
		cin >> oldX[i] >> oldY[i];
	}
	
	// alpha >= 1
	Program lp(CGAL::SMALLER, true, 1, false, 0);
	
	// counts constraints
	int constraint_index = 0;
	
	// No overlaps with the new posters
	for (int i = 0; i < n; ++i) {
		for (int j = i + 1; j < n; ++j) {
			
			// Both possible constraints
			long d_width = 2 * abs(newX[i] - newX[j]);
			long d_height = 2 * abs(newY[i] - newY[j]);
			
			// Check which is less strict
			// -> only one needs to hold, so we use the one that is satisfied lsat
			if (d_width * h > d_height * w) {
				// width has tighter constraint
				lp.set_a(i, constraint_index, w); 
				lp.set_a(j, constraint_index, w); 
				lp.set_b(constraint_index, d_width);
			} else {
				// height has tighter constraint
				lp.set_a(i, constraint_index, h); 
				lp.set_a(j, constraint_index, h); 
				lp.set_b(constraint_index, d_height);
			}
			constraint_index++;
		}
	}
	
	// No overlaps with existing posters
	for (int i = 0; i < n; ++i) {
		
		// Find least strict constraint for variable alpha_i
		long mostStrict_width = LONG_MAX;
		long mostStrict_height = LONG_MAX;
		
		// Go through all placed images
		for (int j = 0; j < m; ++j) {
			
			// Both possible constraints
			long d_width = 2 * abs(newX[i] - oldX[j]) - w;
			long d_height = 2 * abs(newY[i] - oldY[j]) - h;
			
			// Find less strict constraint of the two
			// -> only one of them has to hold
			if (d_width * h > d_height * w) {
				// Check if more strict than before
				// -> all of these constraints need to hold, so we take the strictest one
				mostStrict_width = min(mostStrict_width, d_width);
			} else {
				// Check if more strict than before
				// -> all of these constraints need to hold, so we take the strictest one
				mostStrict_height = min(mostStrict_height, d_height);
			}			
		}
		
		// From these two possible constraints, pick the less strict one
		// -> only one of them has to hold
		// No width constraint was ever set
		if (mostStrict_width == LONG_MAX) {
			// We take the height constraint
			lp.set_a(i, constraint_index, h);
			lp.set_b(constraint_index, mostStrict_height);
		}
		else if (mostStrict_height == LONG_MAX) {
			// We take the width constraint
			lp.set_a(i, constraint_index, w);
			lp.set_b(constraint_index, mostStrict_width);
		}
		else {
			// We check which one is more strict
			// -> both of them need to hold
			if (mostStrict_height * w < mostStrict_width * h) {
				// Height constraint is less strict
				lp.set_a(i, constraint_index, h);
				lp.set_b(constraint_index, mostStrict_height);
			}
			else {
				// Width constraint is less strict
				lp.set_a(i, constraint_index, w);
				lp.set_b(constraint_index, mostStrict_width);
			}
		}
		constraint_index++;
	}
	
	// Set objective
	for (int i = 0; i < n; ++i) {
		lp.set_c(i, -2 * (h + w));
	}
	
	Solution s = CGAL::solve_linear_program(lp, ET());
	assert (s.solves_linear_program(lp));
	cout << setprecision(0) << fixed << ceil_to_double(-s.objective_value()) << endl;
}

int main() {
	int t; cin >> t;
	while(t--)
		testcase();
}