#include <iostream>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;

int floor_to_double(const CGAL::Quotient<ET>& x) {
  double a = floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a + 1 <= x) a += 1;
  return a;
}

int main() {
	while (true) {
		int n, m;
		cin >> n >> m;
		if (n == 0 && m == 0) break;
		
		Program lp(CGAL::LARGER, true, 0, false, 0);
		
		for (int i = 0; i < n; ++i) {
			int min_i, max_i;
			cin >> min_i >> max_i;
			lp.set_b(2*i, min_i);
			lp.set_b(2*i + 1, -max_i);
		}
		
		for (int i = 0; i < m; ++i) {
			int price; cin >> price;
			lp.set_c(i, price);
			for (int j = 0; j < n; ++j) {
				int c; cin >> c;
				lp.set_a(i, 2*j, c);
				lp.set_a(i, 2*j+1, -c);
			}
		}
		
		Solution s = CGAL::solve_nonnegative_linear_program(lp, ET());
		assert(s.solves_linear_program(lp));
		if (s.is_infeasible()) {
			cout << "No such diet." << endl;
		} else {
			assert(s.status() == CGAL::LP_OPTIMAL);
			cout << floor_to_double(s.objective_value()) << endl;
		}
	}
}













