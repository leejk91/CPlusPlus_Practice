#include <iostream>
#include <algorithm>
using namespace std;

//백준 2751 : 수 정렬하기 2


void setting() // 입출력 속도 가속
{
    ios_base::sync_with_stdio(0);
    cin.tie(NULL);
    cout.tie(NULL);

}


int main() {

    setting();

    int N = 0;

    cin >> N;

    int* K = new int[N];

    for (int i = 0; i < N; i++)
    {
        cin >> K[i];
    }



    sort(K, K + N);




    for (int i = 0; i < N; i++)
    {
        cout << K[i] << "\n";
    }

}