#include <iostream>
using namespace std;

int main(void)
{

    // �ʱ�ȭ
    int N = 0;
    long Min = 0;
    long Max = 0;
    cin.tie(NULL);
    ios_base::sync_with_stdio(false);



    while (1)
    {
        cin >> N;
        if (N >= 1 && N <= 1000000)
        {
            break;
        }
        else
            continue;
    }

    int* Dight = new int[N]; // �����迭 �ʱ�ȭ

    for (int k = 0; k <= N - 1; k++) {
        while (1)
        {
            cin >> Dight[k];
            if (Dight[k] <= 1000000 && Dight[k] >= -1000000)
            {
                break;
            }
            else
                continue;
        }

    }
    Min = Dight[0];
    Max = Dight[0];

    for (int i = 1; i <= N - 1; i++)
    {
        if (Min > Dight[i])
            Min = Dight[i];
    }
    for (int i = 1; i <= N - 1; i++)
    {
        if (Max < Dight[i])
            Max = Dight[i];
    }

    cout << Min << " " << Max;

    delete[] Dight; // ���� �迭 ����
    return 0;
}