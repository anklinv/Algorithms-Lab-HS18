#include <iostream>
#include <vector>
#include <climits>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list_traits<vecS, vecS, directedS> Traits;
typedef adjacency_list<vecS, vecS, directedS, no_property, property<edge_capacity_t, long, property<edge_residual_capacity_t, long, property<edge_reverse_t, Traits::edge_descriptor> > > > Graph;
typedef property_map<Graph, edge_capacity_t>::type EdgeCapacityMap;
typedef property_map<Graph, edge_residual_capacity_t>::type ResidualCapacityMap;
typedef property_map<Graph, edge_reverse_t>::type ReverseEdgeMap;
typedef property_map<Graph, vertex_index_t>::type IndexMap;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::vertex_iterator VertexIt;

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
	int l, p;
	cin >> l >> p;
	
	vector<int> g(l);
	vector<int> d(l);
	for (int i = 0; i < l; ++i) {
		cin >> g[i] >> d[i];
	}
	
	Graph G(l);
	EdgeCapacityMap capacityMap = get(edge_capacity, G);
	ReverseEdgeMap revEdgeMap = get(edge_reverse, G);
	ResidualCapacityMap resCapacityMap = get(edge_residual_capacity, G);
	IndexMap index = get(vertex_index, G);
	
	vector<int> inSums(l, 0);
	vector<int> outSums(l, 0);
	for (int i = 0; i < p; ++i) {
		int f, t, c, C;
		cin >> f >> t >> c >> C;
		addEdge(f, t, C - c, capacityMap, revEdgeMap, G);
		inSums[t] += c;
		outSums[f] += c;
	}
	
	for (int i = 0; i < l; ++i) {
		outSums[i] += d[i];
	}
	
	Vertex superS = add_vertex(G);
	Vertex superT = add_vertex(G);
	int totalOutSum = 0;
	VertexIt vbeg, vend;
	for (tie(vbeg, vend) = vertices(G); vbeg != vend; ++vbeg) {
		int i = index[*vbeg];
		if (i >= l) continue;
		addEdge(superS, *vbeg, inSums[i], capacityMap, revEdgeMap, G);
		addEdge(*vbeg, superT, outSums[i], capacityMap, revEdgeMap, G);
		totalOutSum += outSums[i];
	}
	
	// Original source/sink vertices
	Vertex s = add_vertex(G);
	Vertex t = add_vertex(G);
	
	addEdge(t, s, INT_MAX, capacityMap, revEdgeMap, G);
	addEdge(superS, t, totalOutSum, capacityMap, revEdgeMap, G);
	
	for (int i = 0; i < l; ++i) {
		addEdge(s, i, g[i], capacityMap, revEdgeMap, G);
	}
	
	long flow = push_relabel_max_flow(G, superS, superT);
	
	if (flow == totalOutSum)
		cout << "yes" << endl;
	else
		cout << "no" << endl;
}

int main() {
	int t; cin >> t;
	while(t--)
		testcase();
}