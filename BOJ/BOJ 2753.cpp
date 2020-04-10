#include <iostream>
using namespace std;

int main(void) {

    int A;

    while (1) {
        cin >> A;
        if (A >= 1 && A <= 4000)
            break;
        else
            continue;
    }

    if ((A % 4) == 0)
    {
        if ((A % 100) != 0 || (A % 400) == 0)
        {
            cout << 1;
        }
        else
        {
            cout << 0;
        }
    }
    else
    {
        cout << 0;
    }

    return 0;
}