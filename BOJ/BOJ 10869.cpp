#include <iostream>
using namespace std;

int main(void) {

    int A, B;

    while (1) {
        cin >> A >> B;
        if (A >= 1 && B <= 10000)
            break;
        else
            continue;
    }
    cout << A + B << endl
        << A - B << endl
        << A * B << endl
        << A / B << endl
        << A % B;

    return 0;


}