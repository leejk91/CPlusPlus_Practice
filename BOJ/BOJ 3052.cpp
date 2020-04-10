#include <iostream>
using namespace std;

int main(void)
{
    // 백준 3052
    // 초기화
    int A[10] = { 0, }; // 입력받는 숫자
    int B[43] = { 0, }; // 나머지 체크
    int check = 0; // 다른개수를 체크


    for (int i = 0; i < 10; i++)
    {
        cin >> A[i];
    }
    for (int i = 0; i < 10; i++)
    {
        B[A[i] % 42]++;
    }
    
    for (int i = 0; i < 42; i++)
    {
        if (B[i] >= 1 && B[i]<10)
            check++;
    }


    
        
     cout << check;

    return 0;
}