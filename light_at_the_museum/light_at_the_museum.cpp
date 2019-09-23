#include <vector>
#include <iostream>
#include <bitset>
#include <climits>
#include <algorithm>

using namespace std;

struct Result {
	int nr_switches;
	int total_brightness;
	vector<int> switches;
};

vector<Result> possibilities(vector<vector<int>> &on, vector<vector<int>> &off, vector<int>target_brightness, int nr_switches, int nr_rooms){
	vector<Result> result;
	for(int s = 0; s < (1 << nr_switches); ++s){
		int number_switches = 0;
		int total_brightness = 0;
		vector<int> brightness(nr_rooms,0);
		for(int i = 0; i < nr_switches; ++i){	
			if(s&(1<<i)){
				for (int j = 0; j < nr_rooms;++j) {
					brightness[j] += off[i][j];
					total_brightness += off[i][j];
				}
				number_switches++;
			}else{
				for (int j = 0; j < nr_rooms;++j) {
					brightness[j] += on[i][j];
					total_brightness += on[i][j];
				}
			}
		}
		bool legal = true;
		for(int i = 0; i < nr_rooms; ++i){
			if(brightness[i] > target_brightness[i]){
				legal = false;
			}
		}
		if(legal){
			result.push_back({number_switches, total_brightness, brightness});
		}
	}
	return result;
}

bool cmp(const Result a, const Result b) {
	return a.total_brightness < b.total_brightness;
}

void testcase() {
	int nr_switches, nr_rooms;
	cin >> nr_switches >> nr_rooms;
	
	int total_brightness = 0;
	vector<int> target_brightness(nr_rooms);
	for (int i = 0; i < nr_rooms; ++i) {
		cin >> target_brightness[i];
		total_brightness += target_brightness[i];
	}
	
	int firstHalf = nr_switches / 2;
	int secondHalf = nr_switches - firstHalf;
	
	vector<vector<int>> onFirstHalf(firstHalf, vector<int>(nr_rooms));
	vector<vector<int>> onSecondHalf(secondHalf, vector<int>(nr_rooms));
	vector<vector<int>> offFirstHalf(firstHalf, vector<int>(nr_rooms));
	vector<vector<int>> offSecondHalf(secondHalf, vector<int>(nr_rooms));
	for (int i = 0; i < firstHalf; ++i) {
		for (int j = 0; j < nr_rooms; ++j) {
			cin >> onFirstHalf[i][j] >> offFirstHalf[i][j];
		}
	}
	for (int i = 0; i < secondHalf; ++i) {
		for (int j = 0; j < nr_rooms; ++j) {
			cin >> onSecondHalf[i][j] >> offSecondHalf[i][j];
		}
	}
	
	vector<Result> firstHalfResults = possibilities(onFirstHalf, offFirstHalf, target_brightness, firstHalf, nr_rooms);
	vector<Result> secondHalfResults = possibilities(onSecondHalf, offSecondHalf, target_brightness, secondHalf, nr_rooms);
	
	sort(secondHalfResults.begin(), secondHalfResults.end(), cmp);
	
	int min_number_switches = INT_MAX;
	for (Result firstHalfResult : firstHalfResults) {
		Result target = {0, total_brightness - firstHalfResult.total_brightness, vector<int>()};
		auto range = equal_range(secondHalfResults.begin(), secondHalfResults.end(), target, cmp);
		for (auto it = range.first; it != range.second; ++it) {
			// check if valid
			bool valid = true;
			for (int i = 0; i < nr_rooms; ++i) {
				if (firstHalfResult.switches[i] + it->switches[i] != target_brightness[i]) {
					valid = false;
				}
			}
			if (valid && (firstHalfResult.nr_switches + it->nr_switches < min_number_switches)) {
				min_number_switches = firstHalfResult.nr_switches + it->nr_switches;
			}
		}
	}
	
	if(min_number_switches == INT_MAX){
		cout << "impossible" << endl;
	}else{
		cout << min_number_switches << endl;
	}
}

int main() {
	ios_base::sync_with_stdio(false);
	int t; cin >> t;
	while(t--)
		testcase();
}