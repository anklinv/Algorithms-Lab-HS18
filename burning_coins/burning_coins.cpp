#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

int recfunc(vector<int> &v, int start, int end, vector<vector<int>> &memo) {
	if (end - start == 0)
		return v[start];
	else if (end - start == 1)
		return max(v[start], v[end]);
	else {
		if (memo[start + 2][end] == -1) 
			memo[start + 2][end] = recfunc(v, start + 2, end, memo);
		if (memo[start + 1][end - 1] == -1)
			memo[start + 1][end - 1] = recfunc(v, start + 1, end - 1, memo);
		if (memo[start][end - 2] == -1)
			memo[start][end - 2] = recfunc(v, start, end - 2, memo);

		
		return max(v[start] + min(memo[start + 2][end], memo[start +1][end -1]), v[end] + min(memo[start + 1][end -1], memo[start][end - 2]));
	}
}

void testcasefunction() {
	int n;
	cin >> n;
	vector<int> v(n);
	for (int i = 0; i < n; i++) {
		cin >> v[i];
	}
	int result;

	vector<vector<int>> memo(n, vector<int>(n, -1));
	if (n == 0)
		result = 0;
	if (n == 1)
		result = v[0];
	if (n == 2)
		result = max(v[0], v[1]);
	if (n > 2) 
		result = recfunc(v, 0, n-1, memo);
	cout << result << endl;
}

int main() {
	int nr_testcases;
	cin >> nr_testcases;
	for (int i = 0; i < nr_testcases; i++) {
		testcasefunction();
	}
}