#include <iostream>
using namespace std;

int main(void)
{
    // 백준 1546
    // 초기화
    int N = 0;
    int M = 0;
    double sum = 0;

    cin >> N;

    double score[1000];

    for (int i = 0; i < N; i++)
    {
        cin >> score[i];

    }

    for (int i = 0; i < N; i++)
    {
        if (score[i] > M) {
            M = score[i];
        }
    }

    for (int j = 0; j < N; j++)
    {
        score[j] = score[j] / M * 100;
        sum = sum + score[j];
    }

    cout.precision(8);

    cout << sum / N;

    return 0;
}