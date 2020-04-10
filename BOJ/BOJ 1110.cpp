#include <iostream>
using namespace std;

int main(void)
{
    int N = 0;
    int Temp, New = 0;
    int Cycle = 0;
    cin.tie(NULL);
    ios_base::sync_with_stdio(false);
    while (1) {
        cin >> N;
        if (N >= 0 && N <= 99)
        {
            break;
        }
        else
            continue;
    }
    Temp = N;
    New = (Temp / 10) + (Temp % 10);

    do
    {
        if (Temp < 10)
        {
            Temp = ((Temp % 10) * 10) + New;
        }
        else if (Temp >= 10)
        {
            Temp = ((Temp % 10) * 10) + (New % 10);
        }
        New = (Temp / 10) + (Temp % 10);
        Cycle++;

    } while (N != Temp);
    cout << Cycle;
    return 0;
}