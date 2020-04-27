#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>

using namespace std;

//백준 2108

// 최빈값 해결안됨


void setting() // cin과 cout 실행속도 
{
    ios::sync_with_stdio(0); // scanf/ printf랑 cin/cout 싱크를 맞추지 않게
    cin.tie(NULL);
    cout.tie(NULL);
}

void Avgg(vector<int>& arr, int N) // 평균
{
    int sum = 0;

    for (int i = 0; i < N; i++)
    {
        sum = sum + arr[i];
    }

    cout << round(sum / (double)N) << "\n";
}

void Center(vector<int>& arr, int N) // 중앙값
{
    sort(arr.begin(), arr.end());

    cout << arr[N / 2] << "\n";
}

void Counter(int * arr,int N) // 최빈값
{

    int max = 1, max2 = 1, index = 0, index2 = 0;

    for (int i = 0; i <= 8000; i++)
    {
        if (arr[i] > max)
        {
            max = arr[i];
            index = i;
        }
    }

    cout << " 1 : " << index << endl;
    if (max > 1)
    {
        for (int i = 0; i <= 8000; i++)
        {
            if (i != index)
            {
                if (arr[i] == max)
                {
                    max2 = arr[i];
                    index2 = i;
                }
            }
        }
    }

    cout << " 2 : " << index2 << endl;

    if (max > 2 && max2 > 2)
    {
        if (index2 > 4000)
        {
            cout << index2 - 4000 << endl;
        }
        else if (index2 > 0)
        {
            cout << index2 * (-1) << endl;
        }
    }
    
}

void Range(vector<int> & arr, int N) // 범위
{
    sort(arr.begin(), arr.end());

    if (N > 1)
    { 
        int a = 0;
        a = arr.back() - arr.front();
        cout << a;
    }
    else
        cout << "0";

}

int main(void) 
{

    setting();
  
    int N;

    cin >> N; // 수의 갯수 입력

    vector<int> arr(N);
    int C[8001] = { 0, };

    for (int i = 0; i < N; i++)
    {
        int K = 0;
        cin >> K;
        arr[i] = K;
        if (K > 0)
        {
            K = K + 4000;
            C[K]++;
        }
        else
            C[abs(K)]++;
    }

    Avgg(arr,N);
    Center(arr, N);



    Range(arr, N);
}