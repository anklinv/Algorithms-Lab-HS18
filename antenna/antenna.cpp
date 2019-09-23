#include <iostream>
#include <cmath>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>

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
	Traits::Circle c = mc.circle();
	
	K::FT radius = CGAL::sqrt(c.squared_radius());
	
	cout << setprecision(0) << fixed << ceil_to_double(radius) << endl;
}

int main() {
	int n; cin >> n;
	while (n != 0) {
		testcase(n);
		cin >> n;
	}
}