#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp> // Graph
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/biconnected_components.hpp> // articulation_points

using namespace std;
using namespace boost;

namespace boost
{
  struct edge_component_t
  {
    enum
    { num = 555 };
    typedef edge_property_tag kind;
  }
  edge_component;
}

typedef adjacency_list<vecS, vecS, undirectedS, no_property, property<edge_component_t, int> > Graph;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef property_map<Graph, vertex_index_t>::type IndexMap;
typedef graph_traits<Graph>::edge_iterator EdgeIt;
typedef property_map<Graph, edge_component_t>::type ComponentMap;

void testcase() {
	int V, E;
	cin >> V >> E;
	
	Graph G(V);
	for (int i = 0; i < E; i++) {
		int u, v;
		cin >> u >> v;
		
		Edge e; bool success;
		tie(e, success) = add_edge(u, v, G);
	}
	
	IndexMap index = get(vertex_index, G);
	
	ComponentMap component = get(edge_component, G);
	
	int numberOfComponents = biconnected_components(G, component);
	
	vector<int> componentCount(numberOfComponents, 0);
	
	EdgeIt ebeg, eend;
	for (tie(ebeg, eend) = edges(G); ebeg != eend; ++ebeg) {
		componentCount[component[*ebeg]]++;
	}
	
	vector< pair<int, int> > outputList;
	for (tie(ebeg, eend) = edges(G); ebeg != eend; ++ebeg) {
		if (componentCount[component[*ebeg]] == 1) {
			Vertex u = source(*ebeg, G);
			Vertex v = target(*ebeg, G);
			int i, j;
			if (index[u] < index[v]) {
				i = index[u];
				j = index[v];
			} else {
				j = index[u];
				i = index[v];
			}
			outputList.push_back(make_pair(i, j));
		}
	}
	
	// vector<Vertex> art_points;
	// articulation_points(G, back_inserter(art_points));
	
	// vector< pair<int, int> > outputList;
	
	// vector<Edge> alreadySeen;
	
	// for (auto v : art_points) {
		// for (auto u : art_points) {
			// Edge e; bool success;
			// tie(e, success) = edge(u, v, G);
			// if (success) {
				// if (find(alreadySeen.begin(), alreadySeen.end(), e) != alreadySeen.end()) {
					// continue;
				// }
				// alreadySeen.push_back(e);
				// int i, j;
				// if (index[u] < index[v]) {
					// i = index[u];
					// j = index[v];
				// } else {
					// j = index[u];
					// i = index[v];
				// }
				// outputList.push_back(make_pair(i, j));
			// }
		// }
	// }
	
	// EdgeIt ebeg, eend;
	// for (tie(ebeg, eend) = edges(G); ebeg != eend; ++ebeg) {
		// Vertex u = source(*ebeg, G);
		// Vertex v = target(*ebeg, G);
		// if (degree(u, G) == 1 || degree(v, G) == 1) {
			// int i, j;
			// if (index[u] < index[v]) {
				// i = index[u];
				// j = index[v];
			// } else {
				// j = index[u];
				// i = index[v];
			// }
			// outputList.push_back(make_pair(i, j));
		// }
	// }
	
	sort(outputList.begin(), outputList.end());
	
	cout << outputList.size() << endl;
	for (auto el : outputList) {
		cout << el.first << " " << el.second << endl;
	}
}

int main() {
	int c; cin >> c;
	while(c--) {
		testcase();
	}
}