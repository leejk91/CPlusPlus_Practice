#include <iostream>
using namespace std;

int main(void) {

    int A, B;
    while (1) {
        cin >> A >> B;
        if (A >= 100 && B >= 100 && A <= 999 && B <= 999)
            break;
        else
            continue;
    }

    int H = B / 100; // B�� 100�� �ڸ�
    int T = (B - (H * 100)) / 10; // B�� 10�� �ڸ�
    int D = (B - (H * 100) - (T * 10)); // B�� 1�� �ڸ�

    cout << A * D << endl
        << A * T << endl
        << A * H << endl
        << A * B;

}