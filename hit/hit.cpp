#include <iostream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

void testcase(int n) {
  long x,y,a,b;
  cin >> x; cin >> y; cin >> a; cin >> b;

  K::Point_2 o(x,y);
  K::Point_2 d(a,b);

  K::Ray_2 r(o, d);

  bool intersects = false;

  while(n--) {
    long p1, p2, p3, p4;
    cin >> p1; cin >> p2; cin >> p3; cin >> p4;

    K::Point_2 sb(p1, p2);
    K::Point_2 se(p3, p4);

    K::Segment_2 s(sb, se);

    intersects = intersects || CGAL::do_intersect(r, s);
  }

  if (intersects)
    cout << "yes" << endl;
  else
    cout << "no" << endl;
}

int main() {
  int n; cin >> n;
  while (n) {
    testcase(n);
    cin >> n;
  }
}
