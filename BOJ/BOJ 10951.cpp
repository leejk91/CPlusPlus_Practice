#include <iostream>
using namespace std;

int main(void)
{
    int A = 0;
    int B = 0;
    cin.tie(NULL);
    ios_base::sync_with_stdio(false);

    while (1) {
        cin >> A >> B;
        if (cin.eof() == true)
        {
            break;
        }
        else if (A > 0 && B < 10)
        {
            cout << A + B << "\n";
        }
    }


    return 0;
}