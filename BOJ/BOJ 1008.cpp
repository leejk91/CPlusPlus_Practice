#include <iostream>
using namespace std;

int main(void) {
    double A, B;

    while (1) {
        cin >> A >> B;
        if (A > 0 && B < 10)
            break;
        else
            continue;
    }
    cout.precision(10); // 소수점을 10자리까지 표현
    cout << A / B;
    return 0;
}