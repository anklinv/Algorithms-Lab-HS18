#include <iostream>
#include <cmath>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <algorithm>

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef CGAL::Min_circle_2_traits_2<K> Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;

using namespace std;

double ceil_to_double(const K::FT &x) {
	double a = std::ceil(CGAL::to_double(x));
	while (a < x) a += 1;
	while (a-1 >= x) a -= 1;
	return a;
}

void testcase(int n) {
	K::Point_2 points[n];
	for (int i = 0; i < n; i++) {
		long x, y;
		cin >> x; cin >> y;
		points[i] = K::Point_2(x, y);
	}
	
	Min_circle mc (points, points+n, true);
	K::FT minRadius = mc.circle().squared_radius();
	
	for (auto it = mc.support_points_begin(); it != mc.support_points_end(); it++) {
		for (int i = 0; i < n; i++) {
			if (*it == points[i]) {
				
				// Hacky elimination
				int add = i > 0 ? -1 : 1;
				points[i] = points[i+add];
				
				Min_circle smallerMc(points, points+n, true);
			
				Traits::Circle c = smallerMc.circle();
			
				if (c.squared_radius() < minRadius) {
					minRadius = c.squared_radius();
				}
				
				points[i] = *it;
				
				break;
			}
		}
	}
	
	cout << setprecision(0) << fixed << ceil_to_double(CGAL::sqrt(minRadius)) << endl;
}

int main() {
	int n; cin >> n;
	while (n != 0) {
		testcase(n);
		cin >> n;
	}
}