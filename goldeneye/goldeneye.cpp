#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>  Triangulation;
typedef Triangulation::Finite_edges_iterator  FEI;
typedef Triangulation::Vertex_handle Vh;
typedef K::Point_2 Point;

using namespace std;

struct Subset {
	int rank, parent;
};

struct UnionFind {
	UnionFind(int n) {
		subsets.reserve(n);
		for (int i = 0; i < n; ++i) {
			subsets.push_back({0, i});
		}
	}
	
	// Find with path compression
	int find(int i) {
		if (subsets[i].parent != i) {
			subsets[i].parent = find(subsets[i].parent);		
		}
		return subsets[i].parent;
	}
	
	// Union
	void unite(int x, int y) {
		
		// Find roots
		int rootX = find(x);
		int rootY = find(y);
		
		// Attach smaller rank tree under the root of high rank tree
		if (subsets[rootX].rank < subsets[rootY].rank) {
			subsets[rootX].parent = rootY;
		} else if (subsets[rootX].rank > subsets[rootY].rank) {
			subsets[rootY].parent = rootX;
		} else {
			subsets[rootY].parent = rootX;
			subsets[rootX].rank++;
		}
	}
	
	vector<Subset> subsets;
};

struct Edge {
	int u, v;
	K::FT sqLen;
};

void testcase() {
	cout << std::fixed << std::setprecision(0);
	
	
	int n, m; long p;
	cin >> n >> m >> p;
	
	// Read jammers
	vector<pair<Point, int> > jammers;
	jammers.reserve(n);
	for (int i = 0; i < n; ++i) {
		int x, y;
		cin >> x >> y;
		jammers.push_back(make_pair(Point(x, y), i));
	}
	
	// Read in missions
	vector<pair<Point, Point>> missions;
	missions.reserve(m);
	for (int i = 0; i < m; ++i) {
		int x0, y0, x1, y1;
		cin >> x0 >> y0 >> x1 >> y1;
		missions.push_back(make_pair(Point(x0, y0), Point(x1, y1)));
	}

	Triangulation t;
	t.insert(jammers.begin(), jammers.end());
	
	// Turn back to indices and save squared length
	vector<Edge> edges;
	for (FEI ebeg = t.finite_edges_begin(); ebeg != t.finite_edges_end(); ++ebeg) {
		Vh source = ebeg->first->vertex((ebeg->second + 1) % 3);
		Vh target = ebeg->first->vertex((ebeg->second + 2) % 3);
		edges.push_back({source->info(), target->info(), t.segment(*ebeg).squared_length()});
	}
	
	// Sort ascending squared length
	sort(edges.begin(), edges.end(), [] (const Edge &a, const Edge &b) {
		return a.sqLen < b.sqLen;
	});
	
	// Calculate components for initial power
	UnionFind initialPower(n);
	for (auto edge : edges) {
		if (edge.sqLen <= p) {
			initialPower.unite(edge.u, edge.v);
		}
	}
	
	// Go through all missions
	K::FT same = 0;
	K::FT all = 0;
	UnionFind samePower(n);
	UnionFind allPower(n);
	auto sameIt = edges.begin();
	auto allIt = edges.begin();
	
	for (auto mission : missions) {
		
		Vh nearestSource = t.nearest_vertex(mission.first);
		Vh nearestTarget = t.nearest_vertex(mission.second);
		
		// distance such that both starting positions are safe
		K::FT initiallySafe = 4 * max(CGAL::squared_distance(nearestSource->point(), mission.first),
									  CGAL::squared_distance(nearestTarget->point(), mission.second));
		
		// Check if initially safe and in the same components
		if (initiallySafe <= p && initialPower.find(nearestSource->info()) == initialPower.find(nearestTarget->info())) {
			cout << "y";
			
			// update same
			same = max(same, initiallySafe);
			// Go through vector until mission is safe
			for (; sameIt != edges.end() && samePower.find(nearestSource->info()) != samePower.find(nearestTarget->info()); ++sameIt) {
				samePower.unite(sameIt->u, sameIt->v);
			}
		} else {
			cout << "n";
		}
		
		// In any case, update all
		all = max(all, initiallySafe);
		for (; allIt != edges.end() && allPower.find(nearestSource->info()) != allPower.find(nearestTarget->info()); ++allIt) {
			allPower.unite(allIt->u, allIt->v);
		}
	}
	
	// update same and all
	if (sameIt != edges.begin() && (sameIt-1)->sqLen > same) {
		same = (sameIt-1)->sqLen;
	}
	if (allIt != edges.begin() && (allIt-1)->sqLen > all) {
		all = (allIt-1)->sqLen;
	}
	
	cout << endl << all << endl << same << endl;
}

int main(int argc,char* argv[]) {
	int t; cin >> t;
	while(t--)
		testcase();
}