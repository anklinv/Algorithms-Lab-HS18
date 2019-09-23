#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <set>
#include <cassert>

using namespace std;

int sumWords(const map<char, int> &bagOfWords) {
	int sum = 0;
	for (auto letter : bagOfWords) {
		sum += letter.second;
	}
	return sum;
}

bool solution(const vector<pair<char, char> > &pairs, int index, map<char, int> bagOfWords, map< pair<int, map<char, int>>, bool> &memo) {
	if (sumWords(bagOfWords) == 0) {
		return true;
	} else if (index >= pairs.size()) {
		return false;
	} else {
		char choice1, choice2;
		tie(choice1, choice2) = pairs[index];
		if (bagOfWords[choice1] == 0 && bagOfWords[choice2] == 0) {
			// cout << "Skip this" << endl;
			auto key = make_pair(index + 1, bagOfWords);
			if (memo.find(key) == memo.end()) {
				memo[key] = solution(pairs, index + 1, bagOfWords, memo);
			}
			return memo[key];
		} else if (bagOfWords[choice1] > 0 && bagOfWords[choice2] == 0) {
			// cout << "pick first" << endl;
			bagOfWords[choice1]--;
			auto key = make_pair(index + 1, bagOfWords);
			if (memo.find(key) == memo.end()) {
				memo[key] = solution(pairs, index + 1, bagOfWords, memo);
			}
			return memo[key];
		} else if (bagOfWords[choice1] == 0 && bagOfWords[choice2] > 0) {
			// cout << "pick second" << endl;
			bagOfWords[choice2]--;
			auto key = make_pair(index + 1, bagOfWords);
			if (memo.find(key) == memo.end()) {
				memo[key] = solution(pairs, index + 1, bagOfWords, memo);
			}
			return memo[key];
		} else {
			// cout << "try both" << endl;
			map<char, int> bow1 = bagOfWords;
			bow1[choice1]--;
			auto key1 = make_pair(index + 1, bow1);
			map<char, int> bow2 = bagOfWords;
			bow2[choice2]--;
			auto key2 = make_pair(index + 1, bow2);
			if (memo.find(key1) != memo.end() && memo[key1]) {
				return true;
			} else if (memo.find(key2) != memo.end() && memo[key2]) {
				return true;
			}
			if (memo.find(key1) == memo.end()) {
				bool newVal = solution(pairs, index + 1, bow1, memo);
				memo[key1] = newVal;
				if (newVal) return true;
			}
			if (memo.find(key2) == memo.end()) {
				bool newVal = solution(pairs, index + 1, bow2, memo);
				memo[key2] = newVal;
				if (newVal) return true;
			}
			return memo[key1] || memo[key2];
		}
	}
}

void testcase() {
	int h, w; cin >> h >> w;
	string s; cin >> s;
	
	// create bag of words
	map<char, int> bagOfWords;
	int n = s.length();
	for (int i = 0; i < n; ++i) {
		char letter = s[i];
		if (bagOfWords.find(letter) == bagOfWords.end()) {
			bagOfWords[letter] = 1;
		} else {
			bagOfWords[letter]++;
		}
	}
	
	vector<vector<char> > front(h, vector<char>(w));
	vector<vector<char> > back(h, vector<char>(w));
	for (int i = 0; i < h; ++i) {
		string tmp; cin >> tmp;
		for (int j = 0; j < w; ++j) {
			front[i][j] = tmp[j];
		}
	}
	for (int i = 0; i < h; ++i) {
		string tmp; cin >> tmp;
		for (int j = 0; j < w; ++j) {
			back[i][j] = tmp[j];
		}
	}
	
	vector<pair<char, char> > pairs;
	for (int i = 0; i < h; ++i) {
		for (int j = 0; j < w; ++j) {
			pairs.push_back(make_pair(front[i][j], back[i][w-j-1]));
		}
	}
	
	// int m = pairs.size();
	// for (int i = 0; i < m; ++i) {
		
	// }
	
	map< pair<int, map<char, int> >, bool> memo;
	
	if (solution(pairs, 0, bagOfWords, memo)) {
		cout << "Yes" << endl;
	} else {
		cout << "No" << endl;
	}
	
	// for (auto pair : memo) {
		// cout << pair.first.first << endl;
		// for (auto z : pair.first.second) {
			// cout << z.first << ":" << z.second << ",";
		// }
		// cout << endl;
	// }
	// cout << memo.size() << endl;
}

int main() {
	int t; cin >> t;
	while(t--)
		testcase();
}