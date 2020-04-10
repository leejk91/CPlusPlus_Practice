#include <iostream>
using namespace std;

int main(void)
{
    int A, B;
    cin.tie(NULL);
    ios_base::sync_with_stdio(false);

    while (1) {
        cin >> A >> B;
        if (A == 0 && B == 0)
        {
            break;
        }
        else
        {
            if (A > 0 && B < 10)
            {
                cout << A + B << "\n";
            }
            else
                continue;
        }
    }


    return 0;
}