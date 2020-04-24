#include <iostream>
using namespace std;
//백준 2750 : 수 정렬하기

int N = 0;

void setting() // 입출력 속도 가속
{
    ios_base::sync_with_stdio(0);
    cin.tie(NULL);
    cout.tie(NULL);

}

void Sort(int* arr)
{
    int i, j;
    int temp;

    for (i = 1; i < N; i++)
    {
        temp = arr[i];
        for (j = i - 1; j >= 0; j--)
        {
            if (arr[j] > temp)
            {
                arr[j + 1] = arr[j];
            }
            else
                break;

        }
        arr[j + 1] = temp;
    }
}


int main() {

    setting();

    cin >> N;

    int* K = new int[N];

    for (int i = 0; i < N; i++)
    {
        cin >> K[i];
    }

    Sort(K);

    for (int i = 0; i < N; i++)
    {
        cout << K[i] << "\n";
    }
}