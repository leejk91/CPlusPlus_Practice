#include <iostream>
using namespace std;

//백준 10872: 팩토리얼

int sum = 1; // 0!은 1이기 때문이기도 하고 시작할때 0이 곱해지는걸 막기도 위해서

void fac(int n) // 재귀함수 0이 나올때까지 n-1씩 감소하며 곱한다.
{
    if (n == 0)
    {
        return;
    }
    else
    {
        sum = sum * n;
        fac(n - 1);
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

    int N = 0;
    
    cin >> N;

    fac(N);

    cout << sum;
}