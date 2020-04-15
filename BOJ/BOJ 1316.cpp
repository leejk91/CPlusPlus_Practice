#include <iostream>
#include <string>

using namespace std;

//백준 1316
void setting()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);
}

int main(void)
{
    setting();

    string A;
    int T = 0;

    cin >> T;

    int sum = T;

    for (int a = 0; a < T; a++)
    {
        cin >> A;

        bool check[27];

        for (int i = 0; i <= 26; i++)
        {
            check[i] = false;
        }

        for (int i = 0; i < A.size(); i++)
        {
            int W = static_cast<int>(A[i])-97;
            
            if (check[W] == true)
            {
                sum--;
                break;
            }
            else if (check[W] == false)
            {
                if (A[i] != A[i + 1])
                {
                    check[W] = true;
                }
            }

            
        }


    }

    cout << sum;
    return 0;
}