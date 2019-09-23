#include <vector>
#include <iostream>
#include <cstdlib>
#include <map>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// BGL Graph definitions
// ===================== 
// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, Traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > Graph; // new!
// Interior Property Maps
typedef boost::property_map<Graph, boost::edge_capacity_t>::type      EdgeCapacityMap;
typedef boost::property_map<Graph, boost::edge_weight_t >::type       EdgeWeightMap; // new!
typedef boost::property_map<Graph, boost::edge_residual_capacity_t>::type ResidualCapacityMap;
typedef boost::property_map<Graph, boost::edge_reverse_t>::type       ReverseEdgeMap;
typedef boost::graph_traits<Graph>::vertex_descriptor          Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor            Edge;
typedef boost::graph_traits<Graph>::out_edge_iterator  OutEdgeIt; // Iterator

// Custom Edge Adder Class, that holds the references
// to the graph, capacity map, weight map and reverse edge map
// ===============================================================
class EdgeAdder {
    Graph &G;
    EdgeCapacityMap &capacitymap;
    EdgeWeightMap &weightmap;
    ReverseEdgeMap  &revedgemap;

public:
    EdgeAdder(Graph &G, EdgeCapacityMap &capacitymap, EdgeWeightMap &weightmap, ReverseEdgeMap &revedgemap) 
        : G(G), capacitymap(capacitymap), weightmap(weightmap), revedgemap(revedgemap) {}

    void addEdge(int u, int v, long c, long w) {
        Edge e, rev_e;
        boost::tie(e, boost::tuples::ignore) = boost::add_edge(u, v, G);
        boost::tie(rev_e, boost::tuples::ignore) = boost::add_edge(v, u, G);
        capacitymap[e] = c;
        weightmap[e] = w; // new!
        capacitymap[rev_e] = 0;
        weightmap[rev_e] = -w; // new
        revedgemap[e] = rev_e; 
        revedgemap[rev_e] = e;
		
		 // std::cout << "edge from " << boost::source(e, G) << " to " << boost::target(e, G) 
            // << " with capacity " << capacitymap[e] << " and cost " << weightmap[e] << std::endl;
    }
};

using namespace std;

struct Booking {
	int from, to, depart, arrive, profit;
};

const int P = 100;
const int T = 100000;

void testcase() {
	int nr_bookings, nr_stations;
	cin >> nr_bookings >> nr_stations;
	
	vector<int> initialCapacities(nr_stations);
	for (int i = 0; i < nr_stations; ++i) {
		cin >> initialCapacities[i];
	}
	
	vector<set<int> > times(nr_stations);
	vector<Booking> bookings;
	bookings.reserve(nr_bookings);
	for (int i = 0; i < nr_bookings; ++i) {
		int s, t, d, a, p;
		cin >> s >> t >> d >> a >> p;
		bookings.push_back({s-1, t-1, d, a, p});
		times[s-1].insert(d);
		times[t-1].insert(a);
	}
	
	// Insert dummy nodes to have consistent start and end times
	for (int i = 0; i < nr_stations; ++i) {
		times[i].insert(0);
		times[i].insert(T);
	}
	
	// Turn time sets into a map of time step to indices
	vector<map<int, int> > indexMap(nr_stations);
	int vertexIndex = 0;
	for (int i = 0; i < nr_stations; ++i) {
		for (auto time : times[i]) {
			indexMap[i][time] = vertexIndex++;
		}
	}
	
	// Create Graph and Maps
    Graph G(vertexIndex);
	
	// Vertices are defined this way:
	// for station i at timestep t:
	// index = indexMap[i][t];
	
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);
	
	Vertex source = boost::add_vertex(G);
	Vertex target = boost::add_vertex(G);
	
	// Add edges from source to minNodes with initial capacity and cost 0
	for (int i = 0; i < nr_stations; ++i) {
		eaG.addEdge(source, indexMap[i][0], initialCapacities[i], 0);
	}
	
	// Add edges from maxNodes to target with infinite capacity and cost 0
	for (int i = 0; i < nr_stations; ++i) {
		eaG.addEdge(indexMap[i][T], target, INT_MAX, 0);
	}
	
	// Add edges for parking with infinite capacity and cost 0 -> duration * P since we shift
	for (int i = 0; i < nr_stations; ++i) {
		int previousTime;
		int previousIndex;
		for (auto it = indexMap[i].begin(); it != indexMap[i].end(); ++it) {
			if (it == indexMap[i].begin()) {
				previousTime = it->first;
				previousIndex = it->second;
				continue;
			}
			int duration = it->first - previousTime;
			eaG.addEdge(previousIndex, it->second, INT_MAX, duration * P);
			previousTime = it->first;
			previousIndex = it->second;
		}
	}
	
	// Add edges for all bookings with capacity 1 and cost profit -> P - profit since we shift
	for (Booking b : bookings) {
		int duration = b.arrive - b.depart;
		eaG.addEdge(indexMap[b.from][b.depart], indexMap[b.to][b.arrive], 1, duration * P - b.profit);
	}
	
	// Compute flow and cost
	boost::successive_shortest_path_nonnegative_weights(G, source, target);
    int cost = boost::find_flow_cost(G);
	int flow = 0;
    // Iterate over all edges leaving the source to sum up the flow values.
    OutEdgeIt e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(source,G), G); e != eend; ++e) {
        flow += capacitymap[*e] - rescapacitymap[*e];
    }
	
	// Convert costs back
	cout << P*T*flow - cost << endl;
}

int main() {
	int T; cin >> T;
	while(T--)
		testcase();
}