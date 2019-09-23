 #include<iostream>
 #include<algorithm>
 #include<vector>
 #include<array>

 using namespace std;

 void testcasefunction() {

	 //unsigned n;
	 //cin >> n;

	 //vector <vector<int>> X(n, vector<int>(n));
	 //vector <vector<int>> sums(n, vector<int>(n));
	 //// TODO: Why is it so slow with maps?
	 ////map<int, map<int, int>> sums;
	 //int n_pairs = 0;

	 //for (unsigned i = 0; i < n; i++) {
		// for (unsigned j = 0; j < n; j++) {
		//	 cin >> X[i][j];
		// }
	 //}

	 //for (unsigned j = 0; j < n; j++) {
		// int sum = 0;
		// for (unsigned i = 0; i < n; ++i) {
		//	 sum += X[i][j];
		//	 sums[j][i] = sum;
		// }
	 //}

	 int nr_numbers;
	 cin >> nr_numbers;
	 vector<vector<int>> v;
	 vector<vector<int>> partial_sums(nr_numbers, vector<int>(nr_numbers, 0));
	 //for (int i = 0; i < nr_numbers; i++) {
		// vector<int> tmp_row;
		// int sum = 0;
		// int tmp_nr;
		// for (int j = 0; j < nr_numbers; j++) {
		//	 cin >> tmp_nr;
		//	 sum += tmp_nr;
		//	 tmp_row.push_back(tmp_nr);
		// }
		// v.push_back(tmp_row);
	 //}
	 for (int i = 0; i < nr_numbers; i++) {
		 int sum = 0;
		 for (int j = 0; j < nr_numbers; j++) {
			 int tmp_nr;
			 cin >> tmp_nr;
			 sum += tmp_nr;
			 partial_sums[i][j] = sum;
		 }
	 }

	 int evencount = 0;

	 for (int j_start = 0; j_start < nr_numbers; ++j_start) {
		 for (int j_end = j_start; j_end < nr_numbers; ++j_end) {
			 int even = 0;
			 int odd = 0;
			 for (int i = 0; i < nr_numbers; ++i) {
				 if ((partial_sums[i][j_end] - (j_start == 0 ? 0 : partial_sums[i][j_start - 1])) % 2 == 0) {
					 even++;
				 }
				 else {
					 int swap = odd;
					 odd = even;
					 even = swap;
					 odd++;
				 }
				 evencount += even;
			 }
		 }
	 }
	 //vector<vector<int>> partial_sums;
	 //for (int i = 0; i < nr_numbers; i++) {
		// vector<int> tmp_row;
		// int tmp_nr;
		// for (int j = 0; j < nr_numbers; j++) {
		//	 tmp_row.push_back(0);
		// }
		// partial_sums.push_back(tmp_row);
	 //}
	 //partial_sums[0][0] = v[0][0];
	 //int evencount = 0;
	 //for (int i = 1; i < nr_numbers; i++) {
		// partial_sums[0][i] = (partial_sums[0][i - 1] + v[0][i]);
		// partial_sums[i][0] = (partial_sums[i - 1][0] + v[i][0]);
	 //}

	 //for (int i = 1; i < nr_numbers; i++) {
		// for (int j = 1; j < nr_numbers; j++) {
		//	 partial_sums[i][j] = (v[i][j] + partial_sums[i - 1][j] + partial_sums[i][j - 1] - partial_sums[i - 1][j - 1]);
		// }
	 //}
	 //int nr_even = 0;
	 //int nr_odd = 0;
	 //for (int i = 1; i < nr_numbers; i++) {
		// for (int j = 1; j < nr_numbers; j++) {
		//	 if (partial_sums[i][j] % 2 == 0)
		//		 nr_even++;
		// }
	 //}
	 //int o = nr_numbers * nr_numbers - nr_even;
	 //int e = nr_even;

	 //evencount += (e*(e - 1)*(e - 2)*(e - 3)) / 24;
	 //evencount += (o*(o - 1)*(o - 2)*(o - 3)) / 24;
	 //evencount += 6 * (e*(e - 1))*(o*(o - 1)) / 4;
	 //evencount = evencount / 2;


	 /*

	 for (int i1 = 0; i1 < nr_numbers; i1++) {
		 for (int i2 = i1; i2 < nr_numbers; i2++) {
			 for (int j1 = 0; j1 < nr_numbers; j1++) {
				 for (int j2 = j1; j2 < nr_numbers; j2++) {
					 if (i1 == 0 && j1 == 0) {
						 if (partial_sums[i2][j2] % 2 == 0) {
							 evencount++;
						 }
					 }
					 else if (i1 == 0) {
						 if ((partial_sums[i2][j2] - partial_sums[i2][j1 - 1]) % 2 == 0) {
							 evencount++;
						 }
					 }
					 else if (j1 == 0) {
						 if ((partial_sums[i2][j2] - partial_sums[i1 - 1][j2]) % 2 == 0) {
							 evencount++;
						 }
					 }

					 else if ((partial_sums[i2][j2] - partial_sums[i1 - 1][j2] - partial_sums[i2][j1 - 1] + partial_sums[i1 - 1][j1 - 1]) % 2 == 0){
						 evencount++;
					 }
				 }
			 }
		 }

	 }*/
	 cout << evencount << endl;
 }

 int main() {
	 int nr_testcases;
	 cin >> nr_testcases;
	 for (int i = 0; i < nr_testcases; i++) {
		 testcasefunction();
	 }
 }
