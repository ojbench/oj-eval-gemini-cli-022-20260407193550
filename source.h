#include <bits/stdc++.h>

using namespace std;

const int N = 1e4 + 10;
const int mul = 233, Mod = 998244353;

int query(int x, int y, int z);

int A[N];

int guess(int n, int Taskid) {
    memset(A, 0, sizeof A);

    vector<int> I(n);
    for (int i = 0; i < n; i++) I[i] = i + 1;
    
    mt19937 rng(1337);
    shuffle(I.begin(), I.end(), rng);

    vector<int> first5(I.begin(), I.begin() + 5);
    map<vector<int>, int> queries;
    for (int i = 0; i < 5; i++) {
        for (int j = i + 1; j < 5; j++) {
            for (int k = j + 1; k < 5; k++) {
                int q = query(first5[i], first5[j], first5[k]);
                queries[{first5[i], first5[j], first5[k]}] = q;
            }
        }
    }

    vector<int> q_vals;
    for (auto& p : queries) q_vals.push_back(p.second);
    sort(q_vals.begin(), q_vals.end());

    long long S13 = q_vals[0];
    long long S14 = q_vals[1];
    long long S15 = q_vals[4];
    long long S25 = q_vals[8];
    long long S35 = q_vals[9];

    long long x3 = (S13 + S35 - S15) / 2;
    long long x1 = S13 - x3;
    long long x5 = S35 - x3;
    long long x4 = S14 - x1;
    long long x2 = S25 - x5;
    vector<long long> V = {x1, x2, x3, x4, x5};

    vector<int> p(5);
    iota(p.begin(), p.end(), 0);
    vector<long long> best_A_guess(5);
    do {
        vector<long long> A_guess(5);
        for (int i = 0; i < 5; i++) A_guess[p[i]] = V[i];
        
        bool match = true;
        for (int i = 0; i < 5; i++) {
            for (int j = i + 1; j < 5; j++) {
                for (int k = j + 1; k < 5; k++) {
                    long long q_guess = max({A_guess[i], A_guess[j], A_guess[k]}) + min({A_guess[i], A_guess[j], A_guess[k]});
                    if (q_guess != queries[{first5[i], first5[j], first5[k]}]) {
                        match = false;
                        break;
                    }
                }
                if (!match) break;
            }
            if (!match) break;
        }
        if (match) {
            best_A_guess = A_guess;
            break;
        }
    } while (next_permutation(p.begin(), p.end()));

    for (int i = 0; i < 5; i++) {
        A[first5[i]] = best_A_guess[i];
    }

    int p1 = -1, p2 = -1, M = -1;
    for (int i = 0; i < 5; i++) {
        if (best_A_guess[i] == V[0]) p1 = first5[i];
        if (best_A_guess[i] == V[1]) p2 = first5[i];
        if (best_A_guess[i] == V[4]) M = first5[i];
    }

    for (int k = 5; k < n; k++) {
        int z = I[k];
        long long q1 = query(p1, p2, z);
        long long sum_p1_p2 = A[p1] + A[p2];
        
        if (q1 > sum_p1_p2) {
            A[z] = q1 - A[p1];
            if (A[z] > A[M]) {
                M = z;
            }
        } else if (q1 == sum_p1_p2) {
            long long q2 = query(z, p2, M);
            A[z] = q2 - A[M];
            p2 = z;
        } else {
            A[z] = q1 - A[p2];
            p2 = p1;
            p1 = z;
        }
    }

    int ret = 0;
    for (int i = n; i; i--) ret = 1ll * (ret + A[i]) * mul % Mod;
    return ret;
}