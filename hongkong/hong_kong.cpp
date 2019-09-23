# I am fairly certain that I did not finish this one. In my opinion the hardest exercise (also I don't particularly like CGAL).

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>

#include <vector>
#include <iostream>
#include <algorithm>
#include <climits>
#include <cassert>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<int, K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>  Triangulation;
typedef Triangulation::Face_iterator Face_iterator;
typedef Triangulation::Face_handle Face_handle;

typedef boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS,		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
		boost::no_property,				// interior properties of vertices	
		boost::property<boost::edge_weight_t, K::FT> 		// interior properties of edges
		>					Graph;
typedef boost::graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef boost::graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).	
typedef boost::graph_traits<Graph>::edge_iterator		EdgeIt;		// to iterate over all edges
typedef boost::graph_traits<Graph>::out_edge_iterator		OutEdgeIt;	// to iterate over all outgoing edges of a vertex
typedef boost::property_map<Graph, boost::edge_weight_t>::type	WeightMap;	// property map to access the interior property edge_weight_t

typedef K::Point_2 Point;

K::FT MAX = K::FT(LONG_MAX) * K::FT(LONG_MAX)

using namespace std;

// K::FT findTakeOffDist(int i, vector<K::FT> &takeOffDist, vector<bool> &visited, const Graph &G, const Triangulation &t, const WeightMap &w) {
	// if (!visited[i]) {
		// visited[i] = true;
		// K::FT result;
		// bool first = true;
		// OutEdgeIt ebeg, eend;
		// for (boost::tie(ebeg, eend) = out_edges(i, G); ebeg != eend; ++ebeg) {
			// int source = boost::source(*ebeg, G);
			// int target = boost::target(*ebeg, G);
			// int other;
			// if (source != i) {
				// other = source;
			// } else {
				// other = target;
			// }
			// assert(other < takeOffDist.size());
			// assert(other >= 0);
			// K::FT candidate = min(w[*ebeg], findTakeOffDist(other, takeOffDist, visited, G, t, w));
			// if (first || candidate > result) {
				// result = candidate;
				// first = false;
			// }
		// }
		// takeOffDist[i] = result;
	// }
	// return takeOffDist[i];
// }

void testcase() {
	int nr_trees, nr_balloons;
	K::FT tree_radius;
	
	cin >> nr_trees >> nr_balloons >> tree_radius;
	
	vector<Point> trees;
	for (int i = 0; i < nr_trees; ++i) {
		long x, y;
		cin >> x >> y;
		trees.push_back(Point(x, y));
	}
	
	// cout << "read in successful" << endl;
	
	Triangulation t(trees.begin(), trees.end());
	
	int V = 1;
	for (Face_iterator f = t.finite_faces_begin(); f != t.finite_faces_end(); ++f) {
		f->info() = V++;
	}
	
	Graph G(V);
	WeightMap w = boost::get(boost::edge_weight, G);
	
	for (Face_iterator f = t.finite_faces_begin(); f != t.finite_faces_end(); ++f) {
		FT escape = CGAL::squared_radius(f->vertex(0)->point(), f->vertex(1)->point(), f->vertex(2)->point());
		Edge e; bool success;
		tie(e, success) = boost::add_edge(f->info(), 0, G);
		w[e] = escape;
		for (int i = 0; i < 3; ++i) {
			int u = f->info();
			int v;
			if (t.is_infinite(f->neighbor(i))) {
				v = 0;
			} else {
				v = f->neighbor(i)->info();
			}
			Edge e; bool success;
			tie(e, success) = boost::add_edge(u, v, G);
			w[e] = CGAL::squared_distance(f->vertex((i+1)%3)->point(), f->vertex((i+2)%3)->point());
		}
	}
	
	// cout << "graph generation successful" << endl;

	vector<K::FT> takeOffDist(V);
	
	
	// vector<K::FT> takeOffDist(V);
	// vector<bool> takeOffDistIsSet(V, false);
	// takeOffDist[0] = LONG_MAX;
	// takeOffDistIsSet[0] = true;
	// for (int i = 1; i < V; ++i) {
		// takeOffDist[i] = findTakeOffDist(i, takeOffDist, takeOffDistIsSet, G, t, w);
		// // cout << "is set" << endl;
		// // for (auto el : takeOffDistIsSet) cout << el << " ";
		// // cout << endl << "distance" << endl;
		// // for (auto el : takeOffDist) cout << el << " ";
		// // cout << endl;
	// }
	
	// cout << "minDist computation successful" << endl;
	
	for (int i = 0; i < nr_balloons; ++i) {
		long x, y; K::FT s;
		cin >> x >> y >> s;
		Point balloon(x, y);
		
		K::FT testRadius = 4 * K::FT(s + tree_radius) * K::FT(s + tree_radius);
		
		Point nearest = t.nearest_vertex(balloon)->point();
		// Cannot even move
		if (CGAL::squared_distance(nearest, balloon) < (s+tree_radius)*(s+tree_radius)) {
			cout << "n";
		} else {
			// Can directly take off
			if (CGAL::squared_distance(nearest, balloon) >= (2 * s + 2 * tree_radius) * (2 * s + 2 * tree_radius)) {
				cout << "y";
			} 
			// Have to move to a take off location
			else {
				Face_handle f = t.locate(balloon);
				if (t.is_infinite(f)) {
					cout << "y";
				} else {
					int vertex = f->info();
					if (takeOffDist[vertex] >= testRadius) {
						cout << "y";
					} else {
						cout << "n";
					}
				}
			}
		}
	}
	cout << endl;
	
	
	// for (Face_iterator f = t.finite_faces_begin(); f != t.finite_faces_end(); ++f) {
		// K::FT clearance = CGAL::squared_distance(f->vertex(0), f->vertex(1), f->vertex(2));
		// Edge e; bool success;
		// tie(e, success) = boost::add_edge(f->info(), 0, G);
		// w[e] = clearance - ;
	// }
	
  // // read number of points
  // std::size_t n;
  // std::cin >> n;
  // // read points
  // std::vector<K::Point_2> pts;
  // pts.reserve(n);
  // for (std::size_t i = 0; i < n; ++i) {
    // K::Point_2 p;
    // std::cin >> p;
    // pts.push_back(p);
  // }
  // // construct triangulation
  // Triangulation t;
  // t.insert(pts.begin(), pts.end());
  // // output all edges
  // for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e)
    // std::cout << t.segment(e) << "\n";
}

int main() {
	int t; cin >> t;
	while(t--)
		testcase();
}