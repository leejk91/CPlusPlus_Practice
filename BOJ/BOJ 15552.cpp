#include <iostream>
using namespace std;

int main(void)
{
    int T, A, B;
    cin.tie(NULL);
    ios_base::sync_with_stdio(false);

    while (1) {
        cin >> T;
        if (T <= 1000000)
        {
            break;
        }
        else
            continue;
    }
    for (int i = 1; i <= T; i++)
    {
        while (1)
        {
            cin >> A >> B;
            if (A >= 1 && B >= 1 && A <= 1000 && B <= 1000)
            {
                break;
            }
            else
                continue;
        }
        cout << A + B << "\n";
    }
}