#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <limits>
#include <set>

using namespace std;

struct Seq {
    int index;
    int value;
    Seq(int i, int v){
        index = i;
        value = v;
    }
};

void testcasefunction(){
    int nr_words;
    cin >> nr_words;
    vector<int> word_occurrences;
    int temp;
    for(int i = 0; i < nr_words; i++){
        cin >> temp;
        word_occurrences.push_back(temp);
    }

	//vector<Seq> allWords;

	set<pair<int, int>> allWords_set;
	int index = 0;
	for (auto i : word_occurrences) {
		for (int j = 0; j < i; j++) {
			cin >> temp;
			//allWords.push_back(Seq(index, temp));
			allWords_set.insert(make_pair(temp, index));
		}
		index++;
	}

	vector<pair<int, int>> allWords(allWords_set.begin(), allWords_set.end());

	//std::sort(allWords.begin(), allWords.end(), [](Seq &a, Seq &b) {
	//	return a.value < b.value;
	//});
  //  vector<vector<int>> positions;
  //  int counter = 0;
  //  for (auto i: word_occurrences){
		//vector<int> temp_vec;
  //      for (int j = 0; j< i; j++){
  //          cin >> temp;
  //          temp_vec.push_back(temp);
  //      }
  //      positions.push_back(temp_vec);
  //      counter ++;
  //  }

 //   int word_occurences_sum = 0;
 //   for(auto i : word_occurrences){
 //       word_occurences_sum += i;
 //   }
 //   vector<Seq> nextSmallestWord;
 //   vector<int> indices(nr_words, 0);
 //   vector<int> seq_occurrences(nr_words,0);
 //   auto getMin = [&indices, &positions] () {
 //       int minIndex;
 //       int minValue = numeric_limits<int>::max();
 //       for (int i = 0; i < indices.size(); i++) {
	//		if (indices[i] >= positions[i].size()) {
	//			continue;
	//		}
 //           if (positions[i][indices[i]] < minValue) {
 //               minValue = positions[i][indices[i]];
 //               minIndex = i;
 //           }
 //       }
 //       indices[minIndex]++;
 //       return Seq(minIndex, minValue);
 //   };
 //   for(int i = 0; i<word_occurences_sum; i++){
 //       nextSmallestWord.push_back(getMin());
 //   }

	//auto complete = [&seq_occurrences]() {
	//	for (auto el : seq_occurrences) {
	//		if (el < 1) {
	//			return false;
	//		}
	//	}
	//	return true;
	//};
    /*queue<Seq> sequence;
    int i = 0;
    int j = numeric_limits<int>::max();
    int best_i = 0;
    int best_j = numeric_limits<int>::max();
    for(int k=0; k<word_occurences_sum; k++){
        sequence.push(nextSmallestWord[k]);
        seq_occurrences[nextSmallestWord[k].index]++;
        j = sequence.back().value;
        while(seq_occurrences[sequence.front().index] > 1){
            seq_occurrences[sequence.front().index]--;
            sequence.pop();
            i = sequence.front().value;
        }
        if(complete() && j-i < best_j - best_i){
            best_j = j;
            best_i = i;
        }
    }*/
	//queue<Seq> sequence;
	queue<pair<int,int>> sequence;
	vector<int> seq_occurrences(nr_words, 0);

	auto complete = [&seq_occurrences]() {
		for (auto el : seq_occurrences) {
			if (el < 1) {
				return false;
			}
		}
		return true;
	};
	bool COMPLETE = false;
	int i = 0;
	int j = numeric_limits<int>::max();
	int best_i = 0;
	int best_j = numeric_limits<int>::max();
	int numberWords = 0;
	for (int k = 0; k < allWords.size(); k++) {
		j = k;

		//sequence.push(allWords[k]);

		if (!COMPLETE && seq_occurrences[allWords[k].second] == 0) {
			numberWords++;
			if (numberWords == nr_words) {
				COMPLETE = true;
			}
		}

		seq_occurrences[allWords[k].second]++;
		//seq_occurrences[allWords[k].index]++;
		//j = sequence.back().first;
		//j = sequence.back().value;
		while (seq_occurrences[allWords[i].second] > 1) {
		//while (seq_occurrences[sequence.front().second] > 1) {
		//while (seq_occurrences[sequence.front().index] > 1) {
			seq_occurrences[allWords[i].second]--;
			//seq_occurrences[sequence.front().second]--;
			//seq_occurrences[sequence.front().index]--;
			//sequence.pop();
			i++;
			//i = sequence.front().first;
			//i = sequence.front().value;
		}

		if (COMPLETE && allWords[j].first - allWords[i].first < best_j - best_i) {
		//if (COMPLETE && j - i < best_j - best_i) {
			best_j = allWords[j].first;
			best_i = allWords[i].first;
		}
	}
    cout << (best_j-best_i+1) << endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
    int nr_testcases;
    cin >> nr_testcases;
    for(int i = 0; i< nr_testcases; i++){
        testcasefunction();
    }
}