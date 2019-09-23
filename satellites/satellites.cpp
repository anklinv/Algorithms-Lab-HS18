// Includes
// ========
// STL includes
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

// BGL Graph definitions
// =====================
// Graph Type with nested interior edge properties for Flow Algorithms
typedef	boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
	boost::property<boost::edge_capacity_t, long,
		boost::property<boost::edge_residual_capacity_t, long,
			boost::property<boost::edge_reverse_t, Traits::edge_descriptor> > > >	Graph;
// Interior Property Maps
typedef	boost::property_map<Graph, boost::edge_capacity_t>::type		EdgeCapacityMap;
typedef	boost::property_map<Graph, boost::edge_residual_capacity_t>::type	ResidualCapacityMap;
typedef	boost::property_map<Graph, boost::edge_reverse_t>::type		ReverseEdgeMap;
typedef	boost::graph_traits<Graph>::vertex_descriptor			Vertex;
typedef	boost::graph_traits<Graph>::edge_descriptor			Edge;
typedef	boost::graph_traits<Graph>::out_edge_iterator			OutEdgeIt;

using namespace std;

// Custom Edge Adder Class, that holds the references
// to the graph, capacity map and reverse edge map
// ===================================================
class EdgeAdder {
	Graph &G;
	EdgeCapacityMap	&capacitymap;
	ReverseEdgeMap	&revedgemap;

public:
	// to initialize the Object
	EdgeAdder(Graph & G, EdgeCapacityMap &capacitymap, ReverseEdgeMap &revedgemap):
		G(G), capacitymap(capacitymap), revedgemap(revedgemap){}

	// to use the Function (add an edge)
	void addEdge(int from, int to, long capacity) {
		Edge e, rev_e;
		bool success;
		boost::tie(e, success) = boost::add_edge(from, to, G);
		boost::tie(rev_e, success) = boost::add_edge(to, from, G);
		capacitymap[e] = capacity;
		capacitymap[rev_e] = 0;
		revedgemap[e] = rev_e;
		revedgemap[rev_e] = e;
	}
};

void testcase() {
	int g, s, l;
	cin >> g >> s >> l;
	Graph G(g+s);
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);

	for (int i = 0; i < l; ++i) {
		int ground, satellite;
		cin >> ground >> satellite;
		eaG.addEdge(ground, g+satellite, 1); // Offset g
	}
	
	Vertex source = boost::add_vertex(G);
	Vertex target = boost::add_vertex(G);
	
	// Add edge to source
	for (int i = 0; i < g; ++i) {
		eaG.addEdge(source, i, 1);
	}
	// Add edge to target
	for (int i = 0; i < s; ++i) {
		eaG.addEdge(g+i, target, 1);
	}

	// Find a min cut via maxflow
	int flow = boost::push_relabel_max_flow(G, source, target);

	// BFS to find vertex set S
	std::vector<int> vis(g+s+2, false); // visited flags
	std::queue<int> Q; // BFS queue (from std:: not boost::)
	vis[source] = true; // Mark the source as visited
	Q.push(source);
	while (!Q.empty()) {
		const int u = Q.front();
		Q.pop();
		OutEdgeIt ebeg, eend;
		for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) {
			const int v = boost::target(*ebeg, G);
			// Only follow edges with spare capacity
			if (rescapacitymap[*ebeg] == 0 || vis[v]) continue;
			vis[v] = true;
			Q.push(v);
		}
	}
	
	
	// Output S
	vector<int> groundStations;
	vector<int> satellites;
	for (int i = 0; i < g; ++i) {
		// Take all unvisited in L
		if (!vis[i]) {
			groundStations.push_back(i);
		}
	}
	for (int i = g; i < g+s; ++i) {
		// Take all visited in ResidualCapacityMap
		if (vis[i]) {
			satellites.push_back(i);
		}
	}
	
	std::cout << groundStations.size() << " " << satellites.size() << std::endl;
	if (groundStations.size() > 0 || satellites.size() > 0) {
		for (auto groundStation : groundStations) {
			std::cout << groundStation << " ";
		}
		for (auto satellite : satellites) {
			std::cout << satellite - g << " ";
		}
		std::cout << std::endl;
	}
}

// Main
int main() {
	int t; std::cin >> t;
	while(t--)
		testcase();
}