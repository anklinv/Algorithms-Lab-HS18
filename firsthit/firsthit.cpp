#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/intersections.h>
#include <CGAL/squared_distance_2.h>
#include <vector>

using namespace std;

typedef CGAL::Exact_predicates_exact_constructions_kernel K;

double floor_to_double(const K::FT& x)
{
	double a = std::floor(CGAL::to_double(x));
	while (a > x) a -= 1;
	while (a + 1 <= x) a += 1;
	return a;
}


void testcase(int n) {
	long x, y, a, b;
	cin >> x; cin >> y; cin >> a; cin >> b;

	K::Point_2 o(x, y);
	K::Point_2 d(a, b);

	K::Ray_2 r(o, d);
	
	K::Segment_2 r_cheat;
	bool firstHit = true;
	
	vector<K::Segment_2> segments(n);
	
	int i = 0;
	while (n--) {
		long p1, p2, p3, p4;
		cin >> p1; cin >> p2; cin >> p3; cin >> p4;

		K::Point_2 sb(p1, p2);
		K::Point_2 se(p3, p4);

		K::Segment_2 seg(sb, se);
		
		segments[i] = seg;
		i++;
	}
	
	random_shuffle(segments.begin(), segments.end());
	
	for (auto s : segments) {

		boost::optional< boost::variant<K::Point_2, K::Segment_2> > tmp;
		//if(CGAL::do_intersect(r, s)) {
		if (firstHit) {
			tmp = CGAL::intersection(s, r);
		} else {
			tmp = CGAL::intersection(s, r_cheat);
		}
		
		if (tmp) {
			
			if (firstHit) {
				firstHit = false;
				if (const K::Point_2* p = boost::get<K::Point_2>(&*tmp)) {
					r_cheat = K::Segment_2(o, *p);
				} else if (const K::Segment_2* s = boost::get<K::Segment_2>(&*tmp)) {
					if (CGAL::collinear_are_ordered_along_line(o, s->source(), s->target())) {
						r_cheat = K::Segment_2(o, s->source());
					} else {
						r_cheat = K::Segment_2(o, s->target());
					}
				}
			} else {
				if (const K::Point_2* p = boost::get<K::Point_2>(&*tmp)) {
					r_cheat = K::Segment_2(o, *p);
				} else if (const K::Segment_2* s = boost::get<K::Segment_2>(&*tmp)) {
					if (CGAL::collinear_are_ordered_along_line(o, s->source(), s->target())) {
						r_cheat = K::Segment_2(o, s->source());
					} else {
						r_cheat = K::Segment_2(o, s->target());
					}
				}
				
			}
		}
	}
	if (!firstHit) {
		cout << setprecision(0) << fixed << floor_to_double(r_cheat.target().x()) << " " << floor_to_double(r_cheat.target().y()) << endl;
	}
	else {
		cout << "no" << endl;
	}
}

int main() {
	int n; cin >> n;
	while (n) {
		testcase(n);
		cin >> n;
	}
}