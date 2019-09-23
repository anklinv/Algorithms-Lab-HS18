#include <iostream>
#include <vector>
#include <algorithm>
#include <bitset>

using namespace std;

bool valid(int mask, int w) {
    int count = 0;
    for (int i = 0; i < w; ++i) {
        if (mask & (1 << i)) {
            count++;
        }
        else {
            if (count % 2 != 0) {
                return false;
            }
        }
    }
    return true;
}

int max_unset(vector<vector<int> >& dp, int until, int w, int i) {
    int max_val = 0;
    int newint;
    for (int j = 0; j < w; ++j) {
        if ((until & (1 << j))) {
            newint = (until - (1 << j));
            //max_val = newint;
            max_val = max(max_val, dp[i][newint]);
        }

    }
    return max_val;
}

int sum_ones(int bitmask, int w) {
    int sum = 0;
    for (int j = 0; j < w; ++j) {
        if ((bitmask & (1 << j))) {
            sum++;
        }
    }
    return sum;
}

bool compatible(int mask, vector<vector<int> >&table, int w, int i) {
    bitset<17> bitmask(mask);
    for(int j = 0; j < w; ++j){
        if(bitmask[j] && !(table[i][j] && table[i-1][j])){
            return false;
        }
    }
    return true;
}

void testcase() {
    int h, w; cin >> h >> w;
    //cout << "w " << w << " h " << h << endl;
    if (h <= 3 || w <= 3) {
        for(int i = 0; i < h; ++i){
            for (int j = 0; j < w; ++j){
                int a;
                cin >> a;
            }
        }
        cout << 0 << endl;
        return;
    }
    //read in

    vector<vector<int> > table(h-2, vector<int>(w-2,0));
    //vector<bitset<17> > bittable(h,0);
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            int k; cin >> k;
            if(k && (i > 0) && i < (h-1) && (j > 0) && (j<(w-1))){
                table[i-1][j-1] = 1;
            }
        }
    }
    h = h-2;
    w = w-2;

    /*
    vector<vector<int> > table(h, vector<int>(w,0));
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            int k; cin >> k;
            if(k){
                table[i][j] = 1;
            }
        }
    }*/
    vector<bool>validtable(1<<w,false);
    for(int j = 0; j < (1<<w); ++j){
        validtable[j] = valid(j,w);
    }

    vector<vector<int> > dp(h, vector<int>(1 << w, 0));
    for (int i = 1; i < h; ++i) {
        for (int j = 0; j < (1 << w); ++j) {
            if (j == 0) {
                dp[i][j] = dp[i - 1][(1 << w) - 1];
            } else {
                int max_without = max_unset(dp, j, w,i);
                bitset<17>S(j);

                if (validtable[j]) {
                    if (compatible(j, table, w, i)) {
                        int max_with = dp[i - 1][(1 << w) - 1 ^ j] + (sum_ones(j, w) / 2);
                        dp[i][j] = max(max_with, max_without);
                    } else {
                        dp[i][j] = max_without;
                    }
                } else {
                    dp[i][j] = max_without;
                }
            }
        }
    }

/*
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < (1 << w); ++j) {
            cout << dp[i][j] << " ";
        }
        cout << endl;
    }*/
    cout << dp[h-1][((1<<w)-1)] << endl;
}


int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
}
