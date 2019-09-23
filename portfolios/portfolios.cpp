#include <iostream>
#include <vector>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;

int main() {
	while(true) {
		int n, m;
		cin >> n >> m;
		if (n == 0 && m == 0) break;
		
		vector<int> cost(n);
		vector<int> expectedReturn(n);
		for (int i = 0; i < n; ++i) {
			cin >> cost[i] >> expectedReturn[i];
		}
		
		vector<vector<int>> covariance(n, vector<int>(n));
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				cin >> covariance[i][j];
			}
		}
		
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				if (covariance[i][j] != covariance[j][i]) {
					cout << "(i,j) = (" << i << "," << j << ")" << endl;
					cout << covariance[i][j] << endl;
					cout << covariance[j][i] << endl;
				}
			}
		}
		
		for (int k = 0; k < m; ++k) {
			int C, R, V;
			cin >> C >> R >> V;
			
			Program qp(CGAL::SMALLER, true, 0, false, 0);
			
			// maximal cost under C
			for (int i = 0; i < n; ++i) {
				qp.set_a(i, 0, cost[i]);
			}
			qp.set_b(0, C);
			
			// minimum expected return over R
			for (int i = 0; i < n; ++i) {
				qp.set_a(i, 1, -expectedReturn[i]);
			}
			qp.set_b(1, -R);
			
			// set minimizer variance
			for (int i = 0; i < n; ++i) {
				for (int j = i; j < n; ++j) {
					// To make sure that second index is smaller
					qp.set_d(j, i, 2 * covariance[j][i]);
				}
			}
			
			Solution s = CGAL::solve_nonnegative_quadratic_program(qp, ET());
			if (s.is_infeasible()) {
				cout << "No." << endl;
			} else {
				// Check if risk below V
				if (s.objective_value() <= ET(V)) {
					cout << "Yes." << endl;
				} else {
					cout << "No." << endl;
				}
			}
		}
	}
}