// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
#include <queue>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

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

using namespace std;

using namespace boost;

void testcase() {
	int n, i;
	cin >> n >> i;
	Graph G(n);
	WeightMap weightmap = boost::get(boost::edge_weight, G);
	
	vector<vector<int>> weight(n, vector<int>(n));
	for (int j = 1; j < n; ++j) {
		for (int k = 1; k <= n - j; ++k) {
			int cost; cin >> cost;
			Edge e;	bool success;
			boost::tie(e, success) = boost::add_edge(j - 1, j + k - 1, G);
			// cout << "(" << j-1 << "," << j+k-1 << ") " << cost << endl;
			assert(success);
			weight[j-1][j+k-1] = cost;
			weight[j+k-1][j-1] = cost;
			weightmap[e] = cost;
		}
	}
	
	std::vector<Vertex> primpredmap(n);
	boost::prim_minimum_spanning_tree(G, boost::make_iterator_property_map(primpredmap.begin(),
		boost::get(boost::vertex_index, G)), boost::root_vertex(i-1));
	
	Graph MST(n);
	WeightMap w = boost::get(boost::edge_weight, MST);
	
	int mstCost = 0;
	for (int i = 0; i < n; ++i) {
		Edge e; bool success;
		if (i != primpredmap[i]) {
			tie(e, success) = add_edge(i, primpredmap[i], MST);
			// Edge oldE; bool success;
			// tie(oldE, success) = edge(i, primpredmap[i], G);
			// assert(success);
			w[e] = weight[i][primpredmap[i]];
			mstCost += weight[i][primpredmap[i]];
		}
	}
	
	// For all source vertices
	vector<vector<int>> maxCost(n, vector<int>(n, 0));
	for (int u = 0; u < n; ++u) {
		vector<int> vis(n, false);
		std::queue<int> Q;
		vis[u] = true;
		Q.push(u);
		while(!Q.empty()) {
			const int x = Q.front();
			Q.pop();
			OutEdgeIt ebeg, eend;
			for (tie(ebeg, eend) = out_edges(x, MST); ebeg != eend; ++ebeg) {
				const int v = target(*ebeg, MST);
				if (vis[v]) {
					continue;
				}
				maxCost[u][v] = max(w[*ebeg], maxCost[u][x]);
				// maxCost[v][u] = max(w[*ebeg], maxCost[u][x]);
				// if (x == u || w[*ebeg] > maxCost[u][x]) {
					// maxCost[u][v] = w[*ebeg];
				// } else {
					// maxCost[u][v] = maxCost[u][x];
				// }
				vis[v] = true;
				Q.push(v);
			}
		}
	}
	
	// for (int i = 0; i < n; ++i) {
		// for (int j = 0; j < n; ++j) {
			// cout << maxCost[i][j] << " ";
		// }
		// cout << endl;
	// }
	
	int minAddedCost = INT_MAX;
	EdgeIt ebeg, eend;
	for (tie(ebeg, eend) = edges(G); ebeg != eend; ++ebeg) {
		// Make sure not in the MST
		Edge e; bool success;
		int u = source(*ebeg, G);
		int v = target(*ebeg, G);
		if (!(u == primpredmap[v] || v == primpredmap[u])) {		
		// tie(e, success) = edge(u, v, MST);
		// if (!success) {
			minAddedCost = min(minAddedCost, weightmap[*ebeg] - maxCost[u][v]);
		}
	}
	
	cout << mstCost + minAddedCost << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int t; cin >> t;
	while(t--)
		testcase();
}