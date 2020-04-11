#include <iostream>
#include <string>
using namespace std;

// น้มุ 11720

int main(void)
{
    int N = 0, sum = 0;

    while (1) {
        cin >> N;
        if (0 < N && N < 101)
            break;
        continue;

    }

    char* A=new char[N+1];
    
    for (int i = 0; i < N; i++)
    {
        cin >> A[i];
    }

    for (int i = 0; i < N; i++)
    {
        int k = A[i] - '0'; 
        sum = sum + k;
    }

    cout << sum;

    return 0;
}