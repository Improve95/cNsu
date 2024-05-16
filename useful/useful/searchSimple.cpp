#include <iostream>
#include <vector>

using namespace std;

int main() {
    int n;
    cin >> n;
    vector<int> a;

    for (int i = 2; i < n+1; i++) {
        a[i] = i;
    }

    for (int p = 2; p < n; p++) {
        for (int j = p; p < n; j) {
            a[j] = 0;
        }
    }


}