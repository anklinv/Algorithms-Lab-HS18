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
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
		boost::no_property,				// interior properties of vertices	
		boost::property<boost::edge_weight_t, int> 		// interior properties of edges
		>					Graph;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> GraphH;
typedef boost::graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef boost::graph_traits<GraphH>::edge_descriptor		EdgeH;		// Edge Descriptor: an object that represents a single edge.
typedef boost::graph_traits<Graph>::vertex_descriptor	Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).	
typedef boost::graph_traits<GraphH>::vertex_descriptor	VertexH;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).	
typedef boost::graph_traits<Graph>::edge_iterator		EdgeIt;		// to iterate over all edges
typedef boost::graph_traits<Graph>::out_edge_iterator	OutEdgeIt;	// to iterate over all outgoing edges of a vertex
typedef boost::property_map<Graph, boost::edge_weight_t>::type	WeightMap;	// property map to access the interior property edge_weight_t

using namespace std;

void testcases() {
	// Read input
	int positions, slopes, agents, shelters, capacity, enterTime;
	cin >> positions >> slopes >> agents >> shelters >> capacity >> enterTime;
		
	// Create Graph, Vertices
	Graph G(positions);	// creates an empty graph on n vertices
	WeightMap weightmap = boost::get(boost::edge_weight, G);
	
	// Add edges
	for (int i = 0; i < slopes; ++i) {
		int from, to, cost;
		char type;
		cin >> type >> from >> to >> cost;
		
		Edge e;	bool success;
		if (type == 'S') {
			// A slope is just an edge on one direction
			boost::tie(e, success) = boost::add_edge(from, to, G);
			weightmap[e] = cost;
		} else {
			// A skilift is just an edge in both directions
			boost::tie(e, success) = boost::add_edge(from, to, G);
			weightmap[e] = cost;
			boost::tie(e, success) = boost::add_edge(to, from, G);
			weightmap[e] = cost;
		}
	}
	
	// Read agent and shelter positions
	vector<int> agentPositions(agents);
	for (int i = 0; i < agents; ++i) {
		cin >> agentPositions[i];
	}
	vector<int> shelterPositions(shelters);
	for (int i = 0; i < shelters; ++i) {
		cin >> shelterPositions[i];
	}
	
	// For all agent position, compute shortest paths to shelters
	vector<vector<int>> agentToShelterDistance(agents, vector<int>(shelters));
	for (int agentPosition = 0; agentPosition < agents; ++agentPosition) {
		vector<Vertex> predmap(positions);
		vector<int> distmap(positions);
		boost::dijkstra_shortest_paths(G, agentPositions[agentPosition],
			boost::predecessor_map(boost::make_iterator_property_map(predmap.begin(), boost::get(boost::vertex_index, G))).
			distance_map(boost::make_iterator_property_map(distmap.begin(), boost::get(boost::vertex_index, G))));
		for (int shelterPosition = 0; shelterPosition < shelters; ++shelterPosition) {
			agentToShelterDistance[agentPosition][shelterPosition] = distmap[shelterPositions[shelterPosition]];
		}
	}
	
	// Do binary search on possible end times
	int l = 0;
	int r = INT_MAX;
	while (l < r) {
		int middle = l + ((r - l) / 2);
		
		// Build helper graph
		// Note: If capacity = 1, then we just have the normal graph. If capacity = 2, then we duplicate all shelters
		GraphH GH(agents + capacity * shelters);
		for (int a = 0; a < agents; ++a) {
			for (int s = 0; s < shelters; ++s) {
				
				// Find length of shortest path to that shelter
				int shelterDist = agentToShelterDistance[a][s];
				
				// Exclude unreachable
				if (shelterDist != INT_MAX) {
					for (int c = 0; c < capacity; ++c) {
						
						// Check if we could make it to the shelter
						// Note: If capacity is 1, then we just enter normally, but if it is 2, then we can also go in second and be there 2 * enterTime before the avalance is launched. 
						if (shelterDist + (c+1) * enterTime <= middle) {
							EdgeH e;
							boost::tie(e, boost::tuples::ignore) = boost::add_edge(a, agents + c * shelters + s, GH);
						}
					}
				}
			}
		}
		
		// Calculate matching
		std::vector<VertexH> matemap(agents + capacity * shelters);
		boost::edmonds_maximum_cardinality_matching(GH, boost::make_iterator_property_map(matemap.begin(), get(boost::vertex_index, GH)));
		
		// Check if everybody made it
		int matchingsize = matching_size(GH, boost::make_iterator_property_map(matemap.begin(), get(boost::vertex_index, GH)));
		if (matchingsize == agents) {
			r = middle;
		}
		else {
			l = middle + 1;
		}
	}
	assert(l == r);
	cout << l << endl;
}

int main() {
	int t; cin >> t;
	while(t--)
		testcases();
}