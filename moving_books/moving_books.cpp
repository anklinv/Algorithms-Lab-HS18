#include <vector>
#include <iostream>
#include <algorithm>
#include <cassert>

using namespace std;

bool possible(int m, const vector<int> &weight, const vector<int> &strength) {
	
	// cout << "trying " << m << endl;
	int nr_boxes = weight.size();
	int nr_friends = strength.size();
	
	// cout << "i = [0," << (nr_boxes / m) << ")" << endl;
	int i = 0;
	int j = 0;
	
	while (i < nr_friends && j < nr_boxes) {
		if (strength[i] < weight[j]) {
			return false;
		} else {
			i++;
			j+=m;
		}
	}
	return j >= nr_boxes;
	
	
	// int i;
	// for (i = 0; i < (nr_boxes + m - 1) / m; ++i) {
		// // cout << "strength[" << i << "] = " << strength[i] << endl;
		// // cout << "weight[" << i*m + (m-1) << "] = " << weight[i*m + (m-1)] << endl;
		// // cout << "weight[" << i*m << "] = " << weight[i*m] << endl;
		
		// assert(i >= 0);
		// if(i > nr_friends) {
			// break;
			// cout << "------" << endl;
			// cout << "m = " << m << endl;
			// cout << "i = " << i << endl;
			// // cout << "i*m = " << i*m << endl;
			// // cout << "i*m + (m-1) = " << i*m + (m-1) << endl;
			// cout << "nr_friends = " << nr_friends << endl;
			// cout << "i = [0," << (nr_boxes + m - 1) / m << ")" << endl;
			// cout << "------" << endl;
		// }
		// assert(i*m >= 0);
		// assert(i*m < nr_boxes);
		// assert(i*m + (m-1) >= 0);
		// // if(i*m + (m-1) >= nr_boxes) {
			// // cout << "m = " << m << endl;
			// // cout << "i = " << i << endl;
			// // cout << "i*m = " << i*m << endl;
			// // cout << "i*m + (m-1) = " << i*m + (m-1) << endl;
			// // cout << "nr_boxes = " << nr_boxes << endl;
		// // }
		
		// if (strength[i] < weight[i*m] || ((i*m + (m-1) < nr_boxes) && strength[i] < weight[i*m + (m-1)])) {
			// // cout << "returning false" << endl;
			// return false;
		// }
	// }
	// // cout << "returning true" << endl;
	// return (i*m >= nr_boxes);
}

void testcase() {
	int nr_friends, nr_boxes;
	cin >> nr_friends >> nr_boxes;
	
	// cout << "--> " << nr_friends << " " << nr_boxes << endl;
	
	vector<int> strength(nr_friends);
	for (int i = 0; i < nr_friends; ++i) {
		cin >> strength[i];
	}
	
	vector<int> weight(nr_boxes);
	for (int i = 0; i < nr_boxes; ++i) {
		cin >> weight[i];
	}
	
	// Sort descending
	sort(strength.rbegin(), strength.rend());
	sort(weight.rbegin(), weight.rend());
	
	if (*weight.begin() > *strength.begin()) {
		cout << "impossible" << endl;
	} else {
		int l = 1;
		int r = nr_boxes;
		while (l != r) {
			int m = (l+r) / 2;
			if (possible(m, weight, strength)) {
				r = m;
			} else {
				l = m + 1;
			}
		}
		cout << 3 * (l - 1) + 2 << endl;
	}	
}

int main() {
	int t; cin >> t;
	while(t--)
		testcase();
}