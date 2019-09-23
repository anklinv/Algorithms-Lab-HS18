// ALGOLAB BGL Tutorial 2
// Flow example demonstrating
// - interior graph properties for flow algorithms
// - custom edge adder

// Compile and run with one of the following:
// g++ -std=c++11 -O2 flows.cpp -o flows ./flows
// g++ -std=c++11 -O2 -I path/to/boost_1_58_0 flows.cpp -o flows; ./flows

// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>
// Namespaces
using namespace std;
using namespace boost;


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
typedef	boost::graph_traits<Graph>::edge_iterator			EdgeIt;



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
        capacitymap[rev_e] = 0; // reverse edge has no capacity!
        revedgemap[e] = rev_e;
        revedgemap[rev_e] = e;
    }
};


// Functions
// =========
// Function for an individual testcase
void testcases() {
    // Create Graph and Maps
    int w; cin >> w;
    int n; cin >> n;
    if(n==0){
        cout << 0 << endl;
        return;
    }
    Graph G;
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, revedgemap);

    // Add edges
    for(int i = 0; i < n; ++i){
        int from, to; cin >> from >> to;
        if(from < to){
            eaG.addEdge(from + w+1, to, 1);
        }else{
            eaG.addEdge(to + w+1, from, 1);
        }
    }
    for(int i = 1;  i < w; ++i){
        eaG.addEdge(i, i+w+1, 1);
    }
    eaG.addEdge(0, 0+w+1, INT_MAX);

    Vertex source = add_vertex(G);
    Vertex target = add_vertex(G);

    eaG.addEdge(source, 0, INT_MAX);
    eaG.addEdge(w, target, INT_MAX);



    // Calculate flow
    // If not called otherwise, the flow algorithm uses the interior properties
    // - edge_capacity, edge_reverse (read access),
    // - edge_residual_capacity (read and write access).
    long flow1 = boost::push_relabel_max_flow(G, source, target);
    cout << flow1 << endl;

}

// Main function to loop over the testcases
int main() {
    std::ios_base::sync_with_stdio(false);
    int T;	cin >> T;
    for (; T > 0; --T)	testcases();
    return 0;
}