#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <cstdlib>
//#include <assert.h>
#include <tgmath.h>

using namespace std;

void testcase() {
	int n; cin >> n;
	vector<int> l(n, 0);
	for (int i = 0; i < n; i++) {
		cin >> l[i];
	}
	sort(l.begin(), l.end());

	int i = 0;
	int j = 0;
	int count = 0;
	int maxCount = 0;
	int minMaxDistance = 100;
	vector<int> locations;
	auto maxDistance = [&l](int i, int j) {
		int location = l[i] + round(((double)l[j] - l[i]) / 2);
		return max(l[j] - location, location - l[i]);
	};
	for (int k = 0; k < n; k++) {
		//assert(l[k] - l[i] >= 0);
		if (l[k] - l[i] <= 200) {
			count++;
			//cout << count << endl;
			if (count >= maxCount) {
				if (count > maxCount || maxDistance(i,k) < minMaxDistance) {
					locations.clear();
					//assert(locations.size() == 0);
					minMaxDistance = 100;
					maxCount = count;
				}
			
				if (minMaxDistance >= maxDistance(i, k)) {
					int location = l[i] + round(((double)l[k] - l[i]) / 2);
						if (abs(l[k] + l[i]) % 2 == 1) {
							locations.push_back(location - 1);
						}
					locations.push_back(location);
						//if (count == 86)
						//	cout << "l: " << location << " count: " << count << endl;
						minMaxDistance = min(minMaxDistance, maxDistance(i, k));
				}
			}
		}
		else {
			i++;
			count--;
			k--;
		}
	}
	cout << maxCount << " " << minMaxDistance << endl;
	for (auto location : locations) {
		cout << location << " ";
	}
	cout << endl;
}

int main() {
	int t; cin >> t;
	for (int i = 0; i < t; i++) {
		testcase();
	}
}