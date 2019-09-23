#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Team {
	long skill;
	long c;
	Team(long _skill, long _c) : skill(_skill), c(_c) {};
	// bool operator< (const Team &other) {
		// return (this->skill < other.skill) || (this->skill == other.skill && this->c < other.c);
	// };
};

void compute(const vector<int> &skill, int k, int offset, int halfN, vector<Team> &output) {
	
	for (int r = 0; r < (1 << halfN); ++r) {
		int referenceCount = 0;
		for (int i = 0; i < halfN; ++i) {
			if (r & (1<<i)) {
				referenceCount++;
			}
		}
		if (referenceCount > k) continue;
		
		for (int t = 0; t < (1 << halfN - referenceCount); ++t) {
			long skill1 = 0;
			long skill2 = 0;
			int d = 0;
			for (int i = 0; i < halfN; ++i) {
				if (!(r & (1 << i))) {
					if (t & ((1<<i)>>d)) {
						skill1 += skill[offset + i];
					} else {
						skill2 += skill[offset + i];
					}
				} else {
					d++;
				}
			}
			output.push_back(Team(skill1 - skill2, referenceCount));
		}
	}
}

void testcase() {
	int n, k; cin >> n >> k;
	vector<int> skill;
	for (int i = 0; i < n; ++i) {
		int s; cin >> s;
		skill.push_back(s);
	}
	
	int halfN = n/2;
	
	// Compute firstHalf
	vector<Team> firstHalf;
	compute(skill, k, 0, halfN, firstHalf);
	
	// Compute secondHalf
	vector<Team> secondHalf;
	compute(skill, k, halfN, n - halfN, secondHalf);
	
	// Sort second half
	sort(secondHalf.begin(), secondHalf.end(), []  (Team a, Team b) {
		return (a.skill < b.skill) || ((a.skill == b.skill) && (a.c < b.c));
	});
	
	long possibilities = 0;

	// Go over first half
	int firstHalfSize = firstHalf.size();
	for (int i = 0; i < firstHalfSize; ++i) {
		long skill = firstHalf[i].skill;
		long referees = firstHalf[i].c;
		auto bounds = equal_range(secondHalf.begin(), secondHalf.end(), firstHalf[i], [] (const Team a, const Team b) {
			return a.skill < b.skill;
		});
		if (bounds.first == bounds.second) continue;
		// cout << "For skill: " << skill << " we get bounds " << bounds.first->skill << " " << bounds.second->skill << endl;
		Team query = {0, k - referees};
		auto possible = lower_bound(bounds.first, bounds.second, query, [] (const Team a, const Team b) {
			return a.c <= b.c;
		});
		possibilities += possible - bounds.first;
	}
	
	cout << setprecision(0) << fixed << possibilities << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int t; cin >> t;
	while(t--)
		testcase();
}