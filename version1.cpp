// sequential programming approach, which means no parallelization is used.
// to add two matrices A and B, you add their corresponding elements to produce a result matrix C.
// C[i][j]=A[i][j]+B[i][j]

#include <bits/stdc++.h>
#include <chrono>
using namespace std;
#define fl(i, a, n) for (int i = a; i < n; i++)
#define all(p) p.begin(), p.end()
#define vi vector<int>

int main() {

    const int rows = 500, cols = 500;

    vector<vi> A(rows, vi(cols, 1));
    vector<vi> B(rows, vi(cols, 2));
    vector<vi> C(rows, vi(cols));

    auto start = chrono::high_resolution_clock::now();

    fl(i, 0, rows) fl(j, 0, cols) C[i][j] = A[i][j] + B[i][j];

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;   

    cout << "Matrix C (Result of A + B) - Sequential Version" << endl;
    cout << "Time taken: " << duration.count() << " seconds" << endl;
    
    // fl(i, 0, rows) {
    //     fl(j, 0, cols) cout << C[i][j] << " ";
    //     cout << endl;
    // }

    return 0;
}
