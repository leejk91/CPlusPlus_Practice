#include <iostream>
using namespace std;

int main(void)
{
    int T, A, B;
    cin >> T;

    for (int i = 1; i <= T; i++)
    {
        while (1)
        {
            cin >> A >> B;
            if (A > 0 && B < 10)
            {
                break;
            }
            else
                continue;
        }
        cout << A + B << endl;
    }
}