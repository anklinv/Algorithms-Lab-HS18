#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

using namespace std;

struct Info {
	bool color;
	int network;
	bool visited;
	bool operator==(const Info &other) {
		return (this->color == other.color) &&
			   (this->network == other.network) &&
			   (this->visited == other.visited);
	}
};

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<Info, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>  Triangulation;

typedef Triangulation::Vertex_handle Vertex;
typedef Triangulation::Vertex_circulator VertexC;

bool conflict_within_network(const Triangulation &t, K::FT &squaredRange) {
	for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
		if (t.segment(*e).squared_length() <= squaredRange) {
			return true;
		}
	}
	return false;
}

bool try_two_color(const Triangulation &t, K::FT &squaredRange) {
	
	Triangulation network0, network1;
	int component = 0;
	for (auto v = t.finite_vertices_begin(); v != t.finite_vertices_end(); ++v) {
		
		// If not yet visited
		if (!(v->info().visited)) {
			
			v->info().visited = true;
			v->info().network = component;
			
			vector<Vertex> stack;
			stack.push_back(v);
			
			while (!stack.empty()) {
				
				Vertex u = stack.back();
				stack.pop_back();
				
				VertexC c = t.incident_vertices(u);
				
				do {
					if (!t.is_infinite(c)) {
						if (CGAL::squared_distance(u->point(), c->point()) <= squaredRange) {
							if (c->info() == u->info()) {
								return false;
							} 
							
							if (!c->info().visited) {
								stack.push_back(c);
								c->info() = {!u->info().color, component, true};
							}
						}
					}
				} while (++c != t.incident_vertices(u));
			}
			component++;
		}
		
		// Assign to correct network depending on the color
		if (v->info().color) {
			network0.insert(v->point());
		} else {
			network1.insert(v->point());
		}
	}
	
	return !conflict_within_network(network0, squaredRange) && !conflict_within_network(network1, squaredRange);
}

void testcase() {
	int nr_stations, nr_clues; K::FT squaredRange;
	cin >> nr_stations >> nr_clues >> squaredRange;
	
	// Use squared range
	squaredRange *= squaredRange;
	
	Info init = {false, 0, false};
	
	vector<pair<K::Point_2, Info>> stations;
	for (int i = 0; i < nr_stations; ++i) {
		int x, y;
		cin >> x >> y;
		stations.push_back(make_pair(K::Point_2(x, y), init));
	}
	
	Triangulation t(stations.begin(), stations.end());
	
	//               (pun intended)
	bool colorable = try_two_color(t, squaredRange);
	
	for (int i = 0; i < nr_clues; ++i) {
		int x, y;
		cin >> x >> y;
		K::Point_2 holmes(x, y);
		cin >> x >> y;
		K::Point_2 watson(x, y);
		
		if (!colorable) {
			// Not colorable
			cout << "n";
		} else {
			if (CGAL::squared_distance(holmes, watson) <= squaredRange) {
				// Direct communication
				cout << "y";
			} else {
				auto station_holmes = t.nearest_vertex(holmes);
				auto station_watson = t.nearest_vertex(watson);
				if (CGAL::squared_distance(holmes, station_holmes->point()) > squaredRange || CGAL::squared_distance(watson, station_watson->point()) > squaredRange) {
					// Unable to connect
					cout << "n";
				} else {
					if (station_holmes->info().network == station_watson->info().network) {
						// In the same network
						cout << "y";
					} else {
						// In a different network
						cout << "n";
					}
				}
			}
		}
	}
	cout << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int t; cin >> t;
	while(t--)
		testcase();
}