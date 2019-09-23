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
typedef boost::graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef boost::graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).
typedef boost::graph_traits<Graph>::edge_iterator		EdgeIt;		// to iterate over all edges
typedef boost::graph_traits<Graph>::out_edge_iterator		OutEdgeIt;	// to iterate over all outgoing edges of a vertex
typedef boost::property_map<Graph, boost::edge_weight_t>::type	WeightMap;	// property map to access the interior property edge_weight_t

using namespace std;
// Functions
// =========
void testcases() {
    // Create Graph, Vertices and Edges
    // ================================
    int nr_layers; cin >> nr_layers;
    int V = nr_layers*(nr_layers+1)/2;
    Graph G(2*V);	// creates an empty graph on n vertices
    WeightMap weightmap = boost::get(boost::edge_weight, G);	// start by defining property maps for all interior properties defined in Lines 37, 38
    int k =1;
    int count = V-1;
    vector<int> weights(V);
    while(k <= nr_layers){
        for(int i = 0; i < k; ++i){
            int weight; cin >> weight;
            weights[count] = weight;
            Edge e; bool success;
            tie(e, success) = add_edge(count, count + V, G);
            weightmap[e] = weight;
            count --;
        }
        k++;
    }

    k = nr_layers;
    int layer = 0;
    count = 0;
    while(k){
        for(int i = 0; i < k; ++i){
            //add left
            if(i != 0){
                Edge e; bool success;
                tie(e, success) = add_edge(count + V, count -1, G);
                weightmap[e] = 0;
            }
            //add right
            if(i != (k-1)){
                Edge e; bool success;
                tie(e, success) = add_edge(count + V, count + 1, G);
                weightmap[e] = 0;
            }
            //add bottom left
            if(k != nr_layers){
                Edge e; bool success;
                tie(e, success) = add_edge(count + V, count - nr_layers + layer -1, G);
                weightmap[e] = 0;
            }
            //add bottom right
            if(k != nr_layers){
                Edge e; bool success;
                tie(e, success) = add_edge(count + V, count - nr_layers + layer, G);
                weightmap[e] = 0;
            }
            //add top left
            if(i!= 0){
                Edge e; bool success;
                tie(e, success) = add_edge(count + V, count + nr_layers - layer -1, G);
                weightmap[e] = 0;
            }
            //add top right
            if(i!= k-1){
                Edge e; bool success;
                tie(e, success) = add_edge(count + V, count + nr_layers - layer, G);
                weightmap[e] = 0;
            }
            count ++;
        }
        k--;
        layer++;
    }



    // Dijkstra shortest paths
    // =======================
    std::vector<Vertex> predmap1(2*V);	// We will use this vector as an Exterior Property Map: Vertex -> Dijkstra Predecessor
    std::vector<int> distmap1(2*V);		// We will use this vector as an Exterior Property Map: Vertex -> Distance to source
    Vertex start = 0;
    boost::dijkstra_shortest_paths(G, start, // We MUST provide at least one of the two maps
                                   boost::predecessor_map(boost::make_iterator_property_map(predmap1.begin(), boost::get(boost::vertex_index, G))).	// predecessor map as Named Parameter
                                           distance_map(boost::make_iterator_property_map(distmap1.begin(), boost::get(boost::vertex_index, G))));	// distance map as Named Parameter


    // Dijkstra shortest paths
    // =======================
    std::vector<Vertex> predmap2(2*V);	// We will use this vector as an Exterior Property Map: Vertex -> Dijkstra Predecessor
    std::vector<int> distmap2(2*V);		// We will use this vector as an Exterior Property Map: Vertex -> Distance to source
    start = nr_layers-1;
    boost::dijkstra_shortest_paths(G, start, // We MUST provide at least one of the two maps
                                   boost::predecessor_map(boost::make_iterator_property_map(predmap2.begin(), boost::get(boost::vertex_index, G))).	// predecessor map as Named Parameter
                                           distance_map(boost::make_iterator_property_map(distmap2.begin(), boost::get(boost::vertex_index, G))));	// distance map as Named Parameter


    // Dijkstra shortest paths
    // =======================
    std::vector<Vertex> predmap3(2*V);	// We will use this vector as an Exterior Property Map: Vertex -> Dijkstra Predecessor
    std::vector<int> distmap3(2*V);		// We will use this vector as an Exterior Property Map: Vertex -> Distance to source
    start = V-1;
    boost::dijkstra_shortest_paths(G, start, // We MUST provide at least one of the two maps
                                   boost::predecessor_map(boost::make_iterator_property_map(predmap3.begin(), boost::get(boost::vertex_index, G))).	// predecessor map as Named Parameter
                                           distance_map(boost::make_iterator_property_map(distmap3.begin(), boost::get(boost::vertex_index, G))));	// distance map as Named Parameter
    int min_dist = INT_MAX;
    for(int i = V; i < 2*V; ++i){
        //cout << "i " << i << endl;
        //cout << "distmap1 " << distmap1[i] << " distmap2 " << distmap2[i] << " distmap3 " << distmap3[i] << endl;
        min_dist = min(min_dist, distmap1[i]+distmap2[i]+distmap3[i] - 2* weights[i-V]);
        //cout << "sum " << min_dist << endl;
    }

    cout << min_dist << endl;

}

// Main function looping over the testcases
int main() {
    std::ios_base::sync_with_stdio(false); // if you use cin/cout. Do not mix cin/cout with scanf/printf calls!
    int T;	cin >> T;
    while(T--)	testcases();
    return 0;
}
