// ALGOLAB BGL Tutorial 1
// Code snippets demonstrating 
// - graph definitions
// - several algorithms (components, distance-based algorithms, maximum matching)
// - how to pass exterior property maps
// - use of iterators

// Compile and run with one of the following:
// g++ -std=c++11 -O2 bgl-code_snippets.cpp -o bgl-code_snippets; ./bgl-code_snippets
// g++ -std=c++11 -O2 -I path/to/boost_1_58_0 bgl-code_snippets.cpp -o bgl-code_snippets; ./bgl-code_snippets

// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
#include <map>
#include <set>
#include <queue>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>


// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
		boost::no_property,				// interior properties of vertices	
		boost::property<boost::edge_weight_t, int> 		// interior properties of edges
		>					Graph;
typedef boost::graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef boost::graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).	
typedef boost::graph_traits<Graph>::edge_iterator		EdgeIt;		// to iterate over all edges
typedef boost::graph_traits<Graph>::out_edge_iterator		OutEdgeIt;	// to iterate over all outgoing edges of a vertex
typedef boost::property_map<Graph, boost::edge_weight_t>::type	WeightMap;	// property map to access the interior property edge_weight_t

typedef	boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
	boost::property<boost::edge_capacity_t, long,
		boost::property<boost::edge_residual_capacity_t, long,
			boost::property<boost::edge_reverse_t, Traits::edge_descriptor> > > >	Graph2;
// Interior Property Maps
typedef	boost::property_map<Graph2, boost::edge_capacity_t>::type		EdgeCapacityMap;
typedef	boost::property_map<Graph2, boost::edge_residual_capacity_t>::type	ResidualCapacityMap;
typedef	boost::property_map<Graph2, boost::edge_reverse_t>::type		ReverseEdgeMap;
typedef	boost::graph_traits<Graph2>::vertex_descriptor			Vertex2;
typedef	boost::graph_traits<Graph2>::edge_descriptor			Edge2;
typedef	boost::graph_traits<Graph2>::edge_iterator			EdgeIt2;


using namespace std;
using namespace boost;

// Custom Edge Adder Class, that holds the references
// to the graph, capacity map and reverse edge map
// ===================================================
class EdgeAdder {
	Graph2 &G;
	EdgeCapacityMap	&capacitymap;
	ReverseEdgeMap	&revedgemap;

public:
	// to initialize the Object
	EdgeAdder(Graph2 & G, EdgeCapacityMap &capacitymap, ReverseEdgeMap &revedgemap):
		G(G), capacitymap(capacitymap), revedgemap(revedgemap){}

	// to use the Function (add an edge)
	void addEdge(int from, int to, long capacity) {
		Edge2 e, rev_e;
		bool success;
		boost::tie(e, success) = boost::add_edge(from, to, G);
		boost::tie(rev_e, success) = boost::add_edge(to, from, G);
		capacitymap[e] = capacity;
		capacitymap[rev_e] = 0; // reverse edge has no capacity!
		revedgemap[e] = rev_e;
		revedgemap[rev_e] = e;
	}
};

struct Path {
	vector<int> p;
	vector<bool> vis;
	long length = 0;
	bool visited(int t) {
		return vis[t];
	};
	void add(int t, int l) {
		this->vis[t] = true;
		this->p.push_back(t);
		this->length += l;
	}
	Path(int V) {
		vis = vector<bool>(V, false);
	}
};

// Functions
// ========= 
void testcases(int T) {
	int V, E, source, target;
	cin >> V >> E >> source >> target;
	
	// cout << "V=" << V << " E=" << E << " source=" << source << " target=" << target << endl;
	
	Graph G(V);
	WeightMap w = get(edge_weight, G);
	
	map<pair<int, int>, int> capacities;
	map<pair<int, int>, int> minLengths;
	for (int i = 0; i < E; ++i) {
		int a, b, c, d;
		cin >> a >> b >> c >> d;
		
		if (b < a) swap(a, b);
		
		pair<int, int> key = make_pair(a, b);
		if (minLengths.find(key) == minLengths.end() || d < minLengths[key]) {
			minLengths[key] = d;
			capacities[key] = c;
		} else {
			if (d == minLengths[key]) {
				capacities[key] += c;
			}
		}
	}
		
	for (auto p : minLengths) {
		Edge e; bool success;
		tie(e, success) = add_edge(p.first.first, p.first.second, G);
		w[e] = p.second;
	}

	// Find shortest path length from s
	std::vector<Vertex> s_predmap(V);
	std::vector<int> s_distmap(V);
	boost::dijkstra_shortest_paths(G, source, // We MUST provide at least one of the two maps
		boost::predecessor_map(boost::make_iterator_property_map(s_predmap.begin(), boost::get(boost::vertex_index, G))).	// predecessor map as Named Parameter
		distance_map(boost::make_iterator_property_map(s_distmap.begin(), boost::get(boost::vertex_index, G))));	// distance map as Named Parameter
		
	// Find shortest path length from t
	std::vector<Vertex> t_predmap(V);
	std::vector<int> t_distmap(V);
	boost::dijkstra_shortest_paths(G, target, // We MUST provide at least one of the two maps
		boost::predecessor_map(boost::make_iterator_property_map(t_predmap.begin(), boost::get(boost::vertex_index, G))).	// predecessor map as Named Parameter
		distance_map(boost::make_iterator_property_map(t_distmap.begin(), boost::get(boost::vertex_index, G))));	// distance map as Named Parameter
		
	int LENGTH = s_distmap[target];
	
	// Build flow graph
	Graph2 G2(V);
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G2);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G2);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G2);
	EdgeAdder eaG(G2, capacitymap, revedgemap);
	
	// Go over all edges of the graph
	EdgeIt ebeg, eend;
	for (tie(ebeg, eend) = edges(G); ebeg != eend; ++ebeg) {
		int u = boost::source(*ebeg, G);
		int v = boost::target(*ebeg, G);
		int l = w[*ebeg];
		
		if (s_distmap[u] < s_distmap[v]) {
			if (l + s_distmap[u] + t_distmap[v] == LENGTH) {
				// cout << "adding edge from " << u << " to " << v << " with capacity " << capacities[make_pair(u,v)] << endl;
				eaG.addEdge(u, v, capacities[make_pair(u,v)]);
			}
		} else {
			if (l + s_distmap[v] + t_distmap[u] == LENGTH) {
				eaG.addEdge(v, u, capacities[make_pair(u,v)]);
				// cout << "adding edge from " << v << " to " << u << " with capacity " << capacities[make_pair(v,u)] << endl;
			}
		}
	}
	
	// cout << "LENGTH = " << LENGTH << endl;

	// // Do BFS and find all paths
	// set<pair<int, int>> pathEdges;
	// std::queue<Path> Q;
	// Path start(V);
	// start.add(source, 0);
	// Q.push(start);
	// while (!Q.empty()) {
		// Path u = Q.front();
		// Q.pop();
		// int last = u.p.back();
		// // cout << "last = " << last << endl;
		// // if (T==23) {
			// // cout << "PATH = ";
			// // for (auto el : u.p) {
				// // cout << el << " ";
			// // }
			// // cout << endl;
		// // }
		// if (last == target) {
			// // cout << "last == target" << endl;
			// if (u.length == LENGTH) {
				// // cout << "short enough, adding path" << endl;
				// int n = u.p.size();
				// for (int i = 0; i < n-1; ++i) {
					// // cout << "adding " << u.p[i] << " to " << u.p[i+1] << endl;
					// pathEdges.insert(make_pair(u.p[i], u.p[i+1]));
				// }
				// // cout << "finished adding edges" << endl;
			// }
		// } else {
			// OutEdgeIt ebeg, eend;
			// for (tie(ebeg, eend) = out_edges(last, G); ebeg != eend; ++ebeg) {
				// const int v = boost::target(*ebeg, G);
				// // if(T==23) cout << "next " << v << endl;
				// if (u.visited(v)) continue;
				// Path newPath = u;
				// newPath.add(v, w[*ebeg]);
				// Q.push(newPath);
			// }
		// }
	// }

	
	// for (auto edge : pathEdges) {
		// eaG.addEdge(edge.first, edge.second, capacities[edge]);
	// }
	
	long flow = boost::push_relabel_max_flow(G2, source, target);
	
	cout << flow << endl;
}

// Main function looping over the testcases
int main() {
	std::ios_base::sync_with_stdio(false); // if you use cin/cout. Do not mix cin/cout with scanf/printf calls!
	int T;	cin >> T;
	while(T--)	testcases(T);
	return 0;
}