#include <iostream>
#include <cassert>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose exact integral type
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef CGAL::Quotient<ET> SolT;

using namespace std;

double floor_to_double(const SolT& x)
{
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

int main() {
	while(true) {
		int n; cin >> n;
		if (n == 0) break;
		
		int d; cin >> d;
		
		Program lp (CGAL::SMALLER, false, 0, false, 0);
		
		for (int i = 0; i < n; ++i) {
			vector<int> a(d);
			for (int j = 0; j < d; ++j) {
				cin >> a[j];
			}
			int b; cin >> b;
			lp.set_b(i, b);
			
			// compute norm of a
			long norm = 0;
			for (auto el : a) norm += el * el;
			int sqrtnorm = sqrt(norm);
			
			for (int j = 0; j < d; ++j) {
				lp.set_a(j, i, a[j]);
				// cout << "constraint " << i << " var: " << j << " factor: " << a[j] << endl;
			}
			lp.set_a(d, i, sqrtnorm);
			// cout << "constraint " << i << " var: " << d << " factor: " << norm << endl;
		}
		lp.set_c(d, -1);
		lp.set_l(d, true, 0);
		
		Solution s = CGAL::solve_linear_program(lp, ET());
		assert (s.solves_linear_program(lp));
		
		// cout << s << endl;
		
		if (s.is_infeasible()) {
			cout << "none" << endl;
		} else if (s.is_unbounded()) {
			cout << "inf" << endl;
		} else {
			cout << floor_to_double(-s.objective_value()) << endl;
		}
	}
}
