#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list_traits<vecS, vecS, directedS> Traits;
typedef adjacency_list<vecS, vecS, directedS, no_property, property<edge_capacity_t, long, property<edge_residual_capacity_t, long, property<edge_reverse_t, Traits::edge_descriptor> > > > Graph;
typedef property_map<Graph, edge_capacity_t>::type EdgeCapacityMap;
typedef property_map<Graph, edge_residual_capacity_t>::type ResidualCapacityMap;
typedef property_map<Graph, edge_reverse_t>::type ReverseEdgeMap;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::edge_descriptor Edge;

void addEdge(int from, int to, long capacity,
	EdgeCapacityMap &capacityMap,
	ReverseEdgeMap & revEdgeMap,
	Graph &G)
{
	Edge e, rev_e;
	bool success;
	tie(e, success) = add_edge(from, to, G);
	tie(rev_e, success) = add_edge(to, from, G);
	capacityMap[e] = capacity;
	capacityMap[rev_e] = 0;
	revEdgeMap[e] = rev_e;
	revEdgeMap[rev_e] = e;
}

void testcase() {
	int n, m, s;
	cin >> n >> m >> s;
	
	Graph G(n);
	EdgeCapacityMap capacityMap = get(edge_capacity, G);
	ReverseEdgeMap revEdgeMap = get(edge_reverse, G);
	ResidualCapacityMap resCapacityMap = get(edge_residual_capacity, G);

	// super target
	Vertex target = add_vertex(G);
	
	vector<int> stores(s);
	for (int i = 0; i < s; ++i) {
		cin >> stores[i];
		// Edges to super target
		addEdge(stores[i], target, 1, capacityMap, revEdgeMap, G);
	}
	
	for (int i = 0; i < m; ++i) {
		int u, v;
		cin >> u >> v;
		// cout << "Edge from " << u << " to " << v << endl;
		addEdge(u, v, 1, capacityMap, revEdgeMap, G);
		addEdge(v, u, 1, capacityMap, revEdgeMap, G);
	}
	
	long flow = edmonds_karp_max_flow(G, 0, target);
	if (flow == s)
		cout << "yes" << endl;
	else
		cout << "no" << endl;
}

int main() {
	int t; cin >> t;
	while(t--)
		testcase();
}