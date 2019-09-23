#include <iostream>
#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

#ifdef CGAL_USE_GMP
#include <CGAL/Gmpz.h>
typedef CGAL::Gmpz ET;
#else
#include <CGAL/MP_Float.h>
typedef CGAL::MP_Float ET;
#endif

typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

int floor_to_double(const CGAL::Quotient<ET>& x) {
  double a = floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a + 1 <= x) a += 1;
  return a;
}

int ceil_to_double(const CGAL::Quotient<ET>& x) {
  double a = ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a - 1 >= x) a -= 1;
  return a;
}

void testcase(int p, int a, int b) {
	using namespace std;
	
	// min 	a*x^2 - b*y
	// s.t. x, y >= 0
	//		x + y <= 4
	//	    4x + 2y <= a*b
	//	 	-x + y <= 1
	if (p == 1) {
		Program qp(CGAL::SMALLER, true, 0, false, 0);
		const int X = 0;
		const int Y = 1;
		qp.set_a(X, 0, 1); qp.set_a(Y, 0, 1); qp.set_b(0, 4);
		qp.set_a(X, 1, 4); qp.set_a(Y, 1, 2); qp.set_b(1, a*b);
		qp.set_a(X, 2, -1); qp.set_a(Y, 2, 1); qp.set_b(2, 1);
		qp.set_c(Y, -b);
		qp.set_d(X, X, 2 * a);
		Solution s = CGAL::solve_nonnegative_quadratic_program(qp, ET());
		assert(s.solves_quadratic_program(qp));
		if (s.status() == CGAL::QP_INFEASIBLE) {
			cout << "no" << endl;
		} else if (s.status() == CGAL::QP_UNBOUNDED) {
			cout << "unbounded" << endl;
		} else {
			assert(s.status() == CGAL::QP_OPTIMAL);
			cout << floor_to_double(-s.objective_value()) << endl;
		}
	}
	// min  a*x^2 + b*y + w^2
	// s.t. x, y <= 0
	// 		x + y >= -4
	// 	 	4x + 2y + w >= -a*b
	// 		-x + y >= -1
	else {
		Program qp(CGAL::LARGER, false, 0, false, 0);
		const int X = 0;
		const int Y = 1;
		const int Z = 2;
		qp.set_u(X, true, 0);
		qp.set_u(Y, true, 0);
		qp.set_a(X, 0, 1); qp.set_a(Y, 0, 1); qp.set_b(0, -4);
		qp.set_a(X, 1, 4); qp.set_a(Y, 1, 2); qp.set_a(Z, 1, 1); qp.set_b(1, -(a*b));
		qp.set_a(X, 2, -1); qp.set_a(Y, 2, 1); qp.set_b(2, -1);
		qp.set_c(Y, b);
		qp.set_d(X, X, 2 * a);
		qp.set_d(Z, Z, 2 * 1);
		Solution s = CGAL::solve_quadratic_program(qp, ET());
		assert(s.solves_quadratic_program(qp));
		if (s.is_infeasible()) {
			cout << "no" << endl;
		} else if (s.is_unbounded()) {
			cout << "unbounded" << endl;
		} else {
			assert(s.status() == CGAL::QP_OPTIMAL);
			cout << ceil_to_double(s.objective_value()) << endl;
		}
	}
}

int main() {
	using namespace std;
	int p; cin >> p;
	while(p != 0) {
		int a, b;
		cin >> a >> b;
		testcase(p, a, b);
		cin >> p;
	}
}