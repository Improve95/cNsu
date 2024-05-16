#include <iostream>

using namespace std;

int pow(int a, int b) {
    int res = 1;
    while(b > 0) {
        if ((b & 1) == 1) res *= a;
        a *= a;
        b >> 1;
    }

    return res;
}

int main() {
    cout << pow(8, 4);
}