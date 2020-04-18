#include <iostream>
using namespace std;

//백준 10870: 피보나치 수 5

int sum[21] = { 0,1, }; // 피보나치 수열를 저장할 배열
int N = 0; // 찾는 위치를 저장

void fibo(int n) 
{
    if ((N-n) == N )
    {
        return;
    }
    else
    {
        sum[(N - n)+2] = sum[N - n] + sum[(N - n)+1];
        fibo(n - 1);
    }
}



void setting() // 입출력 속도 가속
{
    ios_base::sync_with_stdio(0);
    cin.tie(NULL);
    cout.tie(NULL);

}

int main(void)
{
    setting();

    

    cin >> N;

    fibo(N);

    cout << sum[N];
}