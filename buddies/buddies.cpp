#include <iostream>
#include <vector>
#include <set>
#include <string>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS> Graph;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::vertex_descriptor Vertex;

struct Counter
{
  struct value_type { template<typename T> value_type(const T&) { } };
  void push_back(const value_type&) { ++count; }
  int count = 0;
};

void testcase() {
	int n, c, f;
	cin >> n >> c >> f;
	
	vector< set< string> > preferences(n);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < c; j++) {
			string s; cin >> s;
			preferences[i].insert(s);
		}
	}
	
	Graph G(n);
	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			Counter c;
			set_intersection(preferences[i].begin(), preferences[i].end(), preferences[j].begin(), preferences[j].end(), back_inserter(c));
			if (c.count > f) {
				Edge e; bool success;
				tie(e, success) = add_edge(i, j, G);
			}
		}
	}
	
	vector<Vertex> mate(n);
	edmonds_maximum_cardinality_matching(G, &mate[0]);
	if (matching_size(G, &mate[0]) == (int) n / 2) {
		cout << "not optimal" << endl;
	} else {
		cout << "optimal" << endl;
	}
}

int main() {
	int t; cin >> t;
	while(t--) {
		testcase();
	}
}