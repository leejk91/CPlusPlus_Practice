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
    cout.precision(10); // �Ҽ����� 10�ڸ����� ǥ��
    cout << A / B;
    return 0;
}