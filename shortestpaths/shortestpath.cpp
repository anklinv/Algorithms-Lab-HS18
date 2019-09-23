#include <iostream>
#include <vector>
#include <climits>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/properties.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, directedS, no_property, property<edge_weight_t, int> > Graph;
typedef property_map<Graph, edge_weight_t>::type WeightMap;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::vertex_descriptor Vertex;

int main() {
	int n, m, q;
	cin >> n >> m >> q;
	
	vector<pair<int, int>> locationMap(n);
	for (int i = 0; i < n; i++) {
		int lon, lat;
		cin >> lon >> lat;
		locationMap[i] = make_pair(lon, lat);
	}

	Graph G(n);
	WeightMap weightmap = get(edge_weight, G);
	for (int j = 0; j < m; j++) {
		int a, b, c;
		cin >> a >> b >> c;
		Edge e; bool success;
		tie(e, success) = add_edge(a, b, G);
		weightmap[e] = c;
	}

	for (int k = 0; k < q; k++) {
		int s, t;
		cin >> s >> t;

		vector<int> distmap(n);
		vector<Vertex> predmap(n);
		dijkstra_shortest_paths(G, s, predecessor_map(make_iterator_property_map(predmap.begin(), get(vertex_index, G))).distance_map(make_iterator_property_map(distmap.begin(), get(vertex_index, G))));
		if (distmap[t] == INT_MAX) {
			cout << "unreachable" << endl;
		}
		else {
			cout << distmap[t] << endl;
		}
	}
}