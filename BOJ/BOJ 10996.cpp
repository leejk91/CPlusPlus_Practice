#include <iostream>
using namespace std;

int main(void)
{

    int N;
    cin.tie(NULL);
    ios_base::sync_with_stdio(false);

    while (1)
    {
        cin >> N;
        if (N >= 1 && N <= 100)
        {
            break;
        }
        else
            continue;
    }
    for (int k = 1; k <= N; k++) {

        for (int i = 1; i <= N; i = i + 2)
        {
            cout << "* ";

        }
        cout << "\n";
        for (int j = 2; j <= N; j = j + 2)
        {
            cout << " *";
        }
        cout << "\n";
    }



    return 0;
}