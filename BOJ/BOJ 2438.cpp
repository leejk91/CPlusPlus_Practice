#include <iostream>
using namespace std;

int main(void)
{
    int N;
    cin.tie(NULL);
    ios_base::sync_with_stdio(false);

    while (1) {
        cin >> N;
        if (N >= 1 && N <= 100)
        {
            break;
        }
        else
            continue;
    }
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= i; j++)
        {
            cout << "*";
        }
        cout << "\n";
    }
}