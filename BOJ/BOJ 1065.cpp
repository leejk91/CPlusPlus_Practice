#include <iostream>
using namespace std;

int hansoo(int a)
{

    int count = 0;
    for (int i = 100; i <= a; i++)
    {
        int A = i / 100; // 100�� �ڸ���
        int B = (i % 100) / 10; // 10�� �ڸ���
        int C = (i % 100) % 10; // 1�� �ڸ���

        if (A - B == B - C && B - A == C - B)
        {
            count++;
        }
    }

    return count;
}

int main(void)
{
    int N = 0;

    while (1)
    {
        cin >> N;
        if (N > 0 && N <= 1000) {
            break;
        }
        continue;
    }

    if (N < 100)
        cout << N;
    else if (N >= 100)
    {
        cout << hansoo(N) + 99;
    }

    return 0;
}