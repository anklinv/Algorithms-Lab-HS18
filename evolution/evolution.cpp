#include <map>
#include <unordered_map>
#include <iostream>
#include <string>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <cassert>
#include <stack>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS> Graph;
typedef boost::graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef boost::graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).	
typedef boost::graph_traits<Graph>::edge_iterator		EdgeIt;		// to iterate over all edges
typedef boost::graph_traits<Graph>::out_edge_iterator		OutEdgeIt;	// to iterate over all outgoing edges of a vertex

using namespace std;
using namespace boost;

typedef vector<vector<pair<int, int> > > Queries;

int binary(const int maxAge, const vector<int> &path, const vector<int> &age) {
	int l = 0;
	int r = path.size() - 1;
	while(l != r) {
		int p = (l + r) / 2;
		if (age[path[p]] > maxAge) {
			l = p + 1;
		} else {
			r = p;
		}
	}
	// cout << "return " << age[path[l]] << endl;
	return path[l];
}

void dfs(const int u, const Graph &G, vector<int> &path, const Queries &queries, vector<int> &result, const vector<int> &ageMap) {
	// handle queries
	// cout << "dfs at " << u << endl;
	for (auto query : queries[u]) {
		// cout << "Binary search in path ";
		// for (auto p : path) cout << ageMap[p] << " ";
		// cout << "maxAge " << query.first << endl;
		// cout << "Query was: " << query.first << "," << query.second << endl;
		result[query.second] = binary(query.first, path, ageMap);
	}
	
	OutEdgeIt ebeg, eend;
	for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) {
		int v = boost::target(*ebeg, G);
		path.push_back(v);
		dfs(v, G, path, queries, result, ageMap);
	}
	
	path.pop_back();
}

void testcase() {
	int n, q; cin >> n >> q;
	Graph G(n);
	std::unordered_map<string, int> nameMap;
	vector<string> indexMap(n);
	vector<int> ageMap(n);
	for (int i = 0; i < n; ++i) {
		string name; int age;
		cin >> name >> age;
		nameMap[name] = i;
		indexMap[i] = name;
		ageMap[i] = age;
	}
	
	// Read graph
	for (int i = 0; i + 1 < n; ++i) {
		Edge e; bool success;
		string s, p; cin >> s >> p;
		tie(e, success) = boost::add_edge(nameMap[p], nameMap[s], G);
	}
	
	// Read queries
	vector<vector<pair<int, int> > > queries(n);
	for (int i = 0; i < q; ++i) {
		string name; int maxAge;
		cin >> name >> maxAge;
		// cout << "read query " << nameMap[name] << " " << maxAge << endl;
		queries[nameMap[name]].push_back(make_pair(maxAge, i));
	}
	
	// for (int i = 0; i < n; ++i) {
		// for (int j = 0; j < queries[i].size(); ++j) {
			// cout << "query " << queries[i][j].second << " pos " << i << " max age " << queries[i][j].first << endl;
		// }
	// }
	
	// cout << "Read in successful" << endl;
	// cout << "nameMap" << endl;
	// for (auto pair : nameMap) {
		// cout << pair.first << " " << pair.second << endl;
	// }
	// cout << "indexMap" << endl;
	// for (auto pair : indexMap) {
		// cout << pair.first << " " << pair.second << endl;
	// }
	// cout << "ageMap" << endl;
	// for (auto pair : ageMap) {
		// cout << pair << endl;
	// }
	
	vector<int> result(q);
	
	int root = max_element(ageMap.begin(), ageMap.end()) - ageMap.begin();
	// cout << "root index: " << root << " with name " << indexMap[root] << endl;
	
	// DFS
	vector<int> path;
	// vector<bool> vis(n, false);
	// stack<int> S;
	// S.push(root);
	path.push_back(root);
	dfs(root, G, path, queries, result, ageMap);
	// while (!S.empty()) {
		// int u = S.top();
		// cout << "top is " << u << endl;
		// S.pop();
		// if (!vis[u]) {
			// vis[u] = true;
			// OutEdgeIt ebeg, eend;
			// for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) {
				// int v = boost::target(*ebeg, G);
				// cout << "now accessing " << v << endl;
				// S.push(v);
				// path.push_back(v);
				
				
			// }
		// }
		// path.pop_back();
	// }
	
	// Print result in correct order
	// int c = 0;
	for (int i = 0; i < q; ++i) {
		// cout << "query " << c << " result is index " << index << " which is " << indexMap[index] << endl;
		cout << indexMap[result[i]];
		if (i != q-1) cout << " ";
		// c++;
	}
	cout << endl;
	
	// for (int i = 0; i < q; ++i) {
		// if (i != 0) cout << " ";
		// string name; int maxAge;
		// cin >> name >> maxAge;
		// int nextIndex = nameMap[name];
		// int prevIndex;
		// // Go through graph from name, as long as we are below maxAge
		// do {
			// OutEdgeIt oebeg, oeend;
			// // cout << "while accessing" << endl;
			// boost::tie(oebeg, oeend) = boost::out_edges(nextIndex, G);
			// // We know we have always only one outedge:
			// if (oebeg == oeend) {
				// prevIndex = nextIndex;
				// break;
			// }
			// Vertex next = boost::target(*oebeg, G);
			// prevIndex = nextIndex;
			// nextIndex = index[next];
		// } while (ageMap[nextIndex] <= maxAge);
			
		// // The right node is in prevIndex
		// cout << indexMap[prevIndex];
	// }
	// cout << endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	int T; cin >> T;
	while(T--)
		testcase();
	return 0;
}