#include <iostream>
#include <string>

using namespace std;

//백준 2941
void setting()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);
}

int main(void)
{
    setting();

    string A;

    cin >> A;

    int size = A.size();

    for (int i = 0; i < A.size(); i++)
    {
        if (A[i] == 'c')
        {
            if (A[i + 1] == '=' || A[i + 1] == '-')
            {
                i++;
                size--;
            }

        }
        else if (A[i] == 'd')
        {
            if (A[i + 1] == 'z' && A[i + 2] == '=')
            {
                i += 2;
                size -= 2;
            }
            else if (A[i + 1] == '-')
            {
                i++;
                size--;
            }
        }
        else if (A[i] == 'l' && A[i + 1] == 'j')
        {
            i++;
            size--;
        }
        else if (A[i] == 'n' && A[i + 1] == 'j')
        {
            i++;
            size--;
        }
        else if (A[i] == 's' && A[i + 1] == '=')
        {
            i++;
            size--;
        }
        else if (A[i] == 'z' && A[i + 1] == '=')
        {
            i++;
            size--;
        }
    }

    cout << size;

    return 0;
}