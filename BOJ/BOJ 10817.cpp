#include <iostream>
using namespace std;

int main(void)
{

    int A, B, C;
    int min = 0;
    cin.tie(NULL);
    ios_base::sync_with_stdio(false);

    while (1)
    {
        cin >> A >> B >> C;
        if (A >= 1 && A >= 1 && C >= 1 && A <= 100 && B <= 100 && C <= 100)
        {
            break;
        }
        else
            continue;
    }

    if (A >= B)
    {
        if (B >= C)
        {
            cout << B;
        }
        else if (B <= C)
        {
            if (A >= C)
                cout << C;
            else if (A <= C)
                cout << A;
        }
    }
    else if (A <= B)
    {
        if (A >= C)
        {
            cout << A;
        }
        else if (A <= C)
        {
            if (B >= C)
            {
                cout << C;
            }
            else if (B <= C)
            {
                cout << B;
            }
        }
    }

    return 0;
}