#include <iostream>
#include <string>
using namespace std;

int main(void)
{
    // 백준 4344
    // 초기화
    int A[10] = { 0, }; // 입력받는 숫자
    int B[10] = { 0, }; // 나머지 체크
    int check = 0;


    for (int i = 0; i < 10; i++)
    {
        cin >> A[i];
    }
    for (int i = 0; i < 10; i++)
    {
        int save = 0;
        save = A[i] % 42;
        if (save == 0)
            B[0]++;
        else if (save == 1)
            B[1]++;
        else if (save == 2)
            B[2]++;
        else if (save == 3)
            B[3]++;
        else if (save == 4)
            B[4]++;
        else if (save == 5)
            B[5]++;
        else if (save == 6)
            B[6]++;
        else if (save == 7)
            B[7]++;
        else if (save == 8)
            B[8]++;
        else if (save == 9)
            B[9]++;
    }
    for (int i = 0; i < 10; i++)
    {
        if(b[i]>=1)
            check++
    }
    cout << check;
    return 0;
}