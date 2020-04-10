#include <iostream>
using namespace std;

int main(void)
{

    int N;
    cin.tie(NULL);
    ios_base::sync_with_stdio(false);

    while (1)
    {
        cin >> N;
        if (N >= 1 && N <= 100)
        {
            break;
        }
        else
            continue;
    }

    for (int i = N; i >= 1; i--)
    {
        for (int j = N - i; j > 0; j--)
        {
            cout << " ";
        }
        cout << "*";
        for (int j = 1; j < i; j++)
        {
            cout << "**";
        }
        cout << "\n";
    }

    for (int i = 2; i <= N; i++)
    {
        for (int j = N - i; j > 0; j--)
        {
            cout << " ";
        }
        cout << "*";
        for (int j = 1; j < i; j++)
        {
            cout << "**";
        }
        cout << "\n";
    }


    return 0;
}