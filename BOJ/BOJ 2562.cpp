#include <iostream>
using namespace std;

int main(void)
{

    // �ʱ�ȭ
    long Max = 0;
    long Index = 1;
    cin.tie(NULL);
    ios_base::sync_with_stdio(false);


    int Dight[10] = { 0, };

    for (int k = 0; k <= 8; k++) {

        cin >> Dight[k];

    }

    Max = Dight[0];

    for (int i = 1; i <= 8; i++)
    {
        if (Max < Dight[i])
        {
            Max = Dight[i];
            Index = i + 1; // ���° �������� Ȯ���ϱ� ���� ����
        }
    }

    cout << Max << "\n" << Index;


    return 0;
}