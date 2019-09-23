#include <vector>
#include <iostream>
#include <bitset>
#include <algorithm>

using namespace std;

struct Plank {
	long u, r, d, l;
};

void compute(vector<int> &planks, int from, int halfN, vector<Plank> &out, long maxSide) {
	for (int s = 0; s < 1<<halfN; ++s) {
		for (int r = 0; r < 1<<halfN; ++r) {
			Plank p = {0, 0, 0, 0};
			for (int i = 0; i < halfN; ++i) {
				if (s & 1<<i) {
					if (r & 1<<i) {
						p.u += planks[from + i];
					} else {
						p.r += planks[from + i];
					}
				} else {
					if (r & 1<<i) {
						p.d += planks[from + i];
					} else {
						p.l += planks[from + i];
					}
				}
			}
			if (p.u <= maxSide &&
				p.r <= maxSide &&
				p.d <= maxSide &&
				p.l <= maxSide) {
				out.push_back(p);
			}
		}
	}
}

void testcase() {
	int n; cin >> n;
	vector<int> planks(n);
	for (int i = 0; i < n; ++i) {
		cin >> planks[i];
	}

	long maxLen = 0;
	for (auto plank : planks) {
		maxLen += plank;
	}

	if (maxLen % 4 != 0) {
		cout << 0 << endl;
		return;
	}
	
	long target = maxLen / 4;
	
	int halfN = n / 2;
	
	vector<Plank> firstHalf;
	vector<Plank> secondHalf;
	
	compute(planks, 0, halfN, firstHalf, target);
	compute(planks, halfN, n - halfN, secondHalf, target);
	
	// sort second half
	sort(secondHalf.begin(), secondHalf.end(), [] (Plank a, Plank b) {
		return 	(a.u < b.u) || 
				((a.u == b.u) && (a.r < b.r)) ||
				((a.u == b.u) && (a.r == b.r) && (a.d < b.d)) ||
				((a.u == b.u) && (a.r == b.r) && (a.d == b.d) && (a.l < b.l));
	});
	
	// for (auto el : secondHalf) {
		// cout << el.u << " " << el.r << " " << el.d << " " << el.l << endl;
	// }
	
	long finalCount = 0;
	
	// Go over first half
	for (auto p : firstHalf) {
		Plank query = {target - p.u, target - p.r, target - p.d, target - p.l};
		// cout << "We have: " << endl;
		// cout << p.u << " " << p.r << " " << p.d << " " << p.l << endl;
		// cout << "We look for : " << endl;
		// cout << query.u << " " << query.r << " " << query.d << " " << query.l << endl;
		auto bounds = equal_range(secondHalf.begin(), secondHalf.end(), query, [] (Plank a, Plank b) {
			return 	(a.u < b.u) || 
				((a.u == b.u) && (a.r < b.r)) ||
				((a.u == b.u) && (a.r == b.r) && (a.d < b.d)) ||
				((a.u == b.u) && (a.r == b.r) && (a.d == b.d) && (a.l < b.l));
		});
		
		if (bounds.first != bounds.second) {
			finalCount += bounds.second - bounds.first;
		}
	}
	
	cout << finalCount / 24 << endl;
}

	// int finalCount = 0;
	// for (int s = 0; s < (1<<n)<<n; ++s) {
		// int count0 = 0;
		// int count1 = 0;
		// int count2 = 0;
		// int count3 = 0;
		// // vector<int> v1, v2, v3, v4;
		// for (int i = 0; i < n; ++i) {
			// int query = ((1<<(2*i)) | (1<<(2*i + 1)));
			// if ((s & query)>>(2*i) == 0) {
				// // v1.push_back(planks[i]);
				// count0 += planks[i];
			// } else if ((s & query)>>(2*i) == 1) {
				// count1 += planks[i];
				// // v2.push_back(planks[i]);
			// } else if ((s & query)>>(2*i) == 2) {
				// count2 += planks[i];
				// // v3.push_back(planks[i]);
			// } else {
				// count3 += planks[i];
				// // v4.push_back(planks[i]);
			// }
		// }		
		// if (count0 == count1 && count1 == count2 && count2 == count3) {
			// finalCount++;
		// }
	// }
	// cout << finalCount/24 << endl;
	
	// Go through all possible partitions
	// int finalCount = 0;
	// for (int s = 0; s < 1<<n; ++s) {
		// int oneCount = 0;
		// int zeroCount = 0;
		// for (int i = 0; i < n; ++i) {
			// if (s & 1<<i) {
				// oneCount += planks[i];
			// } else {
				// zeroCount += planks[i];
			// }
		// }
		// if (oneCount == zeroCount) {
			// vector<int> ones;
			// vector<int> zeros;
			// for (int i = 0; i < n; ++i) {
				// if (s & 1<<i) {
					// ones.push_back(planks[i]);
				// } else {
					// zeros.push_back(planks[i]);
				// }
			// }
			// // cout << "ones" << endl;
			// // for (auto one : ones)
				// // cout << one << " ";
			// // cout << endl << "zeros" << endl;
			// // for (auto zero : zeros)
				// // cout << zero << " ";
			// // cout << endl;
			// int onesCount = ones.size();
			// int zerosCount = zeros.size();
			// for (int w = 0; w < 1<<onesCount; ++w) {
				// int count1 = 0;
				// int count0 = 0;
				// for (int i = 0; i < onesCount; ++i) {
					// if (w & 1<<i) {
						// count1 += ones[i];
					// } else {
						// count0 += ones[i];
					// }
				// }
				// if (count1 == count0) {
					// for (int q = 0; q < 1<<zerosCount; ++q) {
						// int one = 0;
						// int zero = 0;
						// for (int i = 0; i < zerosCount; ++i) {
							// if (q & 1<<i) {
								// one += zeros[i];
							// } else {
								// zero += zeros[i];
							// }
						// }
						// if (one == zero) {
							// // cout << "{";
							// // for (int i = 0; i < onesCount; ++i) {
								// // if (w & 1<<i) {
									// // cout << ones[i] << " ";
								// // }
							// // }
							// // cout << "} {";
							// // for (int i = 0; i < onesCount; ++i) {
								// // if (!(w & 1<<i)) {
									// // cout << ones[i] << " ";
								// // }
							// // }
							// // cout << "} {";
							// // for (int i = 0; i < zerosCount; ++i) {
								// // if ((q & 1<<i)) {
									// // cout << zeros[i] << " ";
								// // }
							// // }
							// // cout << "} {";
							// // for (int i = 0; i < zerosCount; ++i) {
								// // if (!(q & 1<<i)) {
									// // cout << zeros[i] << " ";
								// // }
							// // }
							// // cout << "}" << endl;
							// finalCount++;
						// }
					// }
				// }
			// }
		// }
	// }
	// cout << finalCount/24 << endl;
// }

int main() {
	int t; cin >> t;
	while(t--)
		testcase();
}