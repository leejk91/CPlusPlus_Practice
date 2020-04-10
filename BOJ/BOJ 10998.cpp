#include <iostream>
using namespace std;

int main(void) {
    int A, B;
    while (1) {
        cin >> A >> B;
        if (A > 0 && B < 10)
            break;
        else
            continue;
    }
    cout << A * B;
    return 0;
}