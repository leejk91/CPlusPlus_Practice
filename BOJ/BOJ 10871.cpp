#include <iostream>
using namespace std;

int main(void)
{
    int N, X;
    int A = 0;
    cin.tie(NULL);
    ios_base::sync_with_stdio(false);

    while (1) {
        cin >> N >> X;
        if (N >= 1 && X <= 10000)
        {
            break;
        }
        else
            continue;
    }

    for (int i = 0; i < N; i++)
    {
        cin >> A;

        if (A < X)
        {
            cout << A << " ";
        }
    }
    return 0;
}