#include <iostream>
using namespace std;

int main(void)
{
    // ���� 3052
    // �ʱ�ȭ
    int A[10] = { 0, }; // �Է¹޴� ����
    int B[43] = { 0, }; // ������ üũ
    int check = 0; // �ٸ������� üũ


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