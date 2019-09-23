#include <iostream>
#include <vector>
#include <algorithm>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS, no_property, property<edge_weight_t, int> > Graph;
typedef property_map<Graph, edge_weight_t>::type WeightMap;
typedef graph_traits<Graph>::vertex_descriptor   Vertex;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::edge_iterator		EdgeIt;

void testcase() {
	int n, E, s, a, b;
	cin >> n >> E >> s >> a >> b;
	
	vector<Graph> graphs(s);
	vector<WeightMap> wms(s);
	
	for (int i = 0; i < s; i++) {
		graphs[i] = Graph(n);
		wms[i] = get(edge_weight, graphs[i]);
	}
	
	for (int i = 0; i < E; i++) {
		int t1, t2;
		cin >> t1 >> t2;
		
		for (int j = 0; j < s; j++) {
			Edge e; bool success;
			tie(e, success) = add_edge(t1, t2, graphs[j]);
			
			int tmp; cin >> tmp;
			wms[j][e] = tmp;
		}
		
	}
	
	vector<int> h(s);
	for (int i = 0; i < s; i++) {
		cin >> h[i];
	}
	
	Graph G(n);
	WeightMap wm = get(edge_weight, G);
	
	for (int i = 0; i < s; i++) {
		
		vector<Edge> mst;
		kruskal_minimum_spanning_tree(graphs[i], back_inserter(mst));
		
		for (auto edge : mst) {
			Edge e; bool success;
			
			tie(e, success) = add_edge(source(edge, graphs[i]), target(edge, graphs[i]), G);
			wm[e] = get(edge_weight_t(), graphs[i], edge);
		}
		
	}
	
	vector<Vertex> predmapG(n);
	vector<int> distmapG(n);
	
		dijkstra_shortest_paths(G,
								a,
								predecessor_map(make_iterator_property_map(predmapG.begin(), get(	vertex_index, G))).
								distance_map(make_iterator_property_map(distmapG.begin(), get(vertex_index, G)))
								);
	
	cout << distmapG[b] << endl;
}

int main() {
	int t; cin >> t;
	while(t--)
		testcase();
}