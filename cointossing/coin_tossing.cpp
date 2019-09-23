#include <vector>
#include <iostream>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list_traits<vecS, vecS, directedS> Traits;
typedef adjacency_list<vecS, vecS, directedS, no_property, property<edge_capacity_t, long, property<edge_residual_capacity_t, long, property<edge_reverse_t, Traits::edge_descriptor> > > > Graph;
typedef property_map<Graph, edge_capacity_t>::type EdgeCapacityMap;
typedef property_map<Graph, edge_residual_capacity_t>::type ResidualCapacityMap;
typedef property_map<Graph, edge_reverse_t>::type ReverseEdgeMap;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::edge_descriptor Edge;

void addEdge(int from, int to, long capacity,
	EdgeCapacityMap &capacityMap,
	ReverseEdgeMap & revEdgeMap,
	Graph &G)
{
	Edge e, rev_e;
	bool success;
	tie(e, success) = add_edge(from, to, G);
	tie(rev_e, success) = add_edge(to, from, G);
	capacityMap[e] = capacity;
	capacityMap[rev_e] = 0;
	revEdgeMap[e] = rev_e;
	revEdgeMap[rev_e] = e;
}

void testcase() {
	int n, m;
	cin >> n >> m;
	
	Graph G(n);
	EdgeCapacityMap capacityMap = get(edge_capacity, G);
	ReverseEdgeMap revEdgeMap = get(edge_reverse, G);
	ResidualCapacityMap resCapacityMap = get(edge_residual_capacity, G);
	
	Vertex source = add_vertex(G);
	Vertex target = add_vertex(G);
	
	int numberGames = 0;
	
	vector<int> score(n, 0);
	for (int i = 0; i < m; ++i) {
		int a, b, c;
		cin >> a >> b >> c;
		if (c == 1) {
			score[a]++;
		}
		else if (c == 2) {
			score[b]++;
		}
		else {
			// Add edge
			numberGames++;
			Vertex game = add_vertex(G);
			addEdge(source, game, 1, capacityMap, revEdgeMap, G);
			addEdge(game, a, 1, capacityMap, revEdgeMap, G);
			addEdge(game, b, 1, capacityMap, revEdgeMap, G);
		}
	}
	vector<int> finalScore(n);
	for (int i = 0; i < n; ++i) {
		cin >> finalScore[i];
	}
	
	int sumScores = 0;
	for (int i = 0; i < n; ++i) {
		sumScores += finalScore[i] - score[i];
	}
	if (sumScores != numberGames) {
		cout << "no" << endl;
		return;
	}
	
	for (int i = 0; i < n; ++i) {
		Edge e; bool success;
		// Avoid having negative edges
		if (finalScore[i] < score[i]) {
			cout << "no" << endl;
			return;
		}
		addEdge(i, target, finalScore[i] - score[i], capacityMap, revEdgeMap, G);
	}
	
	long flow = push_relabel_max_flow(G, source, target);
	
	if (flow == numberGames) {
		cout << "yes" << endl;
	} else {
		cout << "no" << endl;
	}
}

int main() {
	int t; cin >> t;
	while(t--)
		testcase();
}