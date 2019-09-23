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
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>
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

	int h, w; cin >> h >> w;
	string s; cin >> s;
	
	// create bag of words
	vector<int> goal(26);
	int n = s.length();
	for (int i = 0; i < n; ++i) {
		char letter = s[i];
		assert((int) letter - (int) 'A' >= 0);
		assert((int) letter - (int) 'A' <= 25);
		goal[(int) letter - (int) 'A']++;
	}
	
	vector<vector<char> > front(h, vector<char>(w));
	vector<vector<char> > back(h, vector<char>(w));
	for (int i = 0; i < h; ++i) {
		string tmp; cin >> tmp;
		for (int j = 0; j < w; ++j) {
			front[i][j] = tmp[j];
		}
	}
	for (int i = 0; i < h; ++i) {
		string tmp; cin >> tmp;
		for (int j = 0; j < w; ++j) {
			back[i][j] = tmp[j];
		}
	}
	
	// cout << "read stuff" << endl;
	
	vector<vector<int>> pairs(26, vector<int>(26, 0));
	for (int i = 0; i < h; ++i) {
		for (int j = 0; j < w; ++j) {
			// if((int) front[i][j] - (int) 'A' < 0) {
				// cout << front[i][j] << endl;
			// }
			// if((int) back[i][w-j-1] - (int) 'A' < 0) {
				// cout << back[i][w-j-1] << endl;
			// }
			// if((int) front[i][j] - (int) 'A' > 25) {
				// cout << front[i][j] << endl;
			// }
			// if((int) back[i][w-j-1] - (int) 'A' > 25) {
				// cout << back[i][w-j-1] << endl;
			// }
			pairs[(int) front[i][j] - (int) 'A'][(int) back[i][w-j-1] - (int) 'A']++;
		}
	}
	
	// cout << "compute pairs " << endl;
	
	Graph G(26);
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);
	
	Vertex source = add_vertex(G);
	Vertex target = add_vertex(G);
	
	for (int i = 0; i < 26; ++i) {
		eaG.addEdge(i, target, goal[i]);
	}
	
	// cout << "created graph" << endl;
	
	for (int i = 0; i < 26; ++i) {
		for (int j = 0; j < 26; ++j) {
			Vertex choice = add_vertex(G);
			eaG.addEdge(source, choice, pairs[i][j]);
			eaG.addEdge(choice, i, pairs[i][j]);
			eaG.addEdge(choice, j, pairs[i][j]);
		}
	}
	
	long flow = push_relabel_max_flow(G, source, target);
	if (flow == n) {
		cout << "Yes" << endl;
	} else {
		cout << "No" << endl;
	}
	
}

// Main function to loop over the testcases
int main() {
	std::ios_base::sync_with_stdio(false);
	int T; cin >> T;
	while(T--)
		testcases();
}