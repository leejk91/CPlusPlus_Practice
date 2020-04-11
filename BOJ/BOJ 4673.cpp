#include <iostream>
using namespace std;
bool b[10001] = { 0, };

int d(int n)
{
    return n + (n / 1000) + (n % 1000 / 100) + (n % 100 / 10) + (n % 10);
}   

int main(void)
{


    for (int i = 0; i <= 10000; i++)
    {
        
        int k = d(i);
        b[k] = 1;
    }

    for (int i = 0; i <= 10000; i++)
    {
        if (b[i] == 0)
            cout << i << "\n";
    }

    return 0;
}