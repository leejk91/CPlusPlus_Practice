#include <iostream>
using namespace std;

int main(void)
{
    int N;
    cin.tie(NULL);
    ios_base::sync_with_stdio(false);

    while (1) {
        cin >> N;
        if (N <= 100000)
        {
            break;
        }
        else
            continue;
    }
    for (int i = 1; i <= N; i++)
    {
        cout << i << "\n";
    }
}