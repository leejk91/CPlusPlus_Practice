#include <iostream>
#include <string>
using namespace std;

//백준 2675

int main(void)
{
    string P;
    int T = 0, R = 0;

    cin >> T;
    for (int i = 0; i < T; i++)
    {
        cin >> R;
        cin >> P;
        int size = P.size();

        for (int j = 0; j < size; j++)
        {
            for (int k = 0; k < R; k++)
            {
                cout << P[j];
            }
        }
        cout << "\n";
    }

    return 0;
}