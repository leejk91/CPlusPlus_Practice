#include <iostream>
using namespace std;

int main(void)
{
    int n;
    int sum = 0;
    while (1)
    {
        cin >> n;
        if (n >= 1 && n <= 10000)
        {
            break;
        }
        else
            continue;
    }

    for (int i = 1; i <= n; i++)
    {
        sum = sum + i;
    }
    cout << sum;
}