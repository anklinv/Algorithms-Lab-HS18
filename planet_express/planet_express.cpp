#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <climits>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/strong_components.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list < vecS, vecS, directedS, no_property, property < edge_weight_t, int > > Graph;
typedef graph_traits < Graph >::vertex_descriptor Vertex;
typedef graph_traits < Graph >::edge_descriptor Edge;
typedef property_map<Graph, edge_weight_t>::type WeightMap;

void testcase() {
	int V, E, k, T;
	cin >> V >> E >> k >> T;
	
	Graph GT(V);
	WeightMap wt = get(edge_weight, GT);
	
	set<int> teleportation;
	for (int i = 0; i < T; ++i) {
		int t; cin >> t;
		teleportation.insert(t);
	}
	
	for (int i = 0; i < E; ++i) {
		int u, v, c;
		cin >> u >> v >> c;
		Edge e; bool success;
		tie(e, success) = add_edge(v, u, GT); //transposed graph
		wt[e] = c;
	}
		
	vector<int> component(V);
	int nr_components = strong_components(GT, make_iterator_property_map(component.begin(), get(vertex_index, GT)));
	
	vector<vector<int>> reachable(nr_components);
	for (auto t : teleportation) {
		reachable[component[t]].push_back(t);
	}
		
	for (auto l : reachable) {
		Vertex teleporter = add_vertex(GT);
		int cost = l.size() - 1;
		for (auto r : l) {
			Edge toTeleporter; bool success1;
			tie(toTeleporter, success1) = add_edge(vertex(r, GT), teleporter, GT);
			wt[toTeleporter] = cost;
			Edge fromTeleporter; bool success2;
			tie(fromTeleporter, success2) = add_edge(teleporter, vertex(r, GT), GT);
			wt[fromTeleporter] = 0;
		}
	}

	vector<Vertex> predecessors(V+nr_components);
	vector<int> distances(V+nr_components);
	dijkstra_shortest_paths(GT, V - 1,
                          predecessor_map(make_iterator_property_map(predecessors.begin(), get(vertex_index, GT))).
                          distance_map(make_iterator_property_map(distances.begin(), get(vertex_index, GT))));	
	int minDistance = INT_MAX;
	for (int i = 0; i < k; ++i) {
		minDistance = min(minDistance, distances[i]);
	}
	if (minDistance <= 1000000)
		cout << minDistance << endl;
	else
		cout << "no" << endl;
}

int main() {
	int t; cin >> t;
	while(t--)
		testcase();
}