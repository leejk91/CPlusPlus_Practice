#include <iostream>

using namespace std;

//백준 1712 손익분기점


void setting()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);
}

int main(void)
{
    setting();

    int A = 0, B = 0, C = 0;

    cin >> A >> B >> C;

    int i = 0;

    if (C <= B)
    {
        cout << -1;
    }
    else if (C > B)
    {
        cout<<(A / (C - B)) + 1;
    }

   

    return 0;
}