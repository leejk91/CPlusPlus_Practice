#include <iostream>
#include <string>

using namespace std;

//백준 5622


int main(void)
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    string A;

    getline(cin, A);

    int size = A.size();
    int second = 0;

    for (int i = 0; i < size; i++)
    {
        int check = static_cast<int>(A[i]);

        if (check <= 67)
            second += 3;
        else if (check <= 70)
            second += 4;
        else if (check <= 73)
            second += 5;
        else if (check <= 76)
            second += 6;
        else if (check <= 79)
            second += 7;
        else if (check <= 83)
            second += 8;
        else if (check <= 86)
            second += 9;
        else if (check <= 90)
            second += 10;

    }

    cout << second << "\n";

    return 0;
}