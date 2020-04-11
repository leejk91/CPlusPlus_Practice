#include <iostream>
#include <string>
using namespace std;

//น้มุ 10809

int main(void)
{
    string S;
    int B[26];

    for (int i = 0; i < 26; i++)
    {
        B[i] = -1;
    }

    cin >> S;

    int L = S.size();

    for (int i = 0; i <= L; i++)
    {
        if (B[S[i] - 'a'] == -1)
        {
            B[S[i] - 'a'] = i;
        }
        else
            ;
    }

    for (int i = 0; i < 26; i++)
    {
        cout << B[i] << " ";
    }


    return 0;
}