#include <iostream>
#include <string>
using namespace std;

//백준 1157

int main(void)
{
    string P;
    int A[101];

    for (int i = 0; i <= 100; i++)
    {
        A[i] = 0;
    }

    cin >> P;

    int size = P.size();

    for (int i = 0; i < size; i++)
    {

        if (static_cast<int>(P[i]) >= 97 && static_cast<int>(P[i]) <= 122)
            A[static_cast<int>(P[i]) - 32]++;
        else
            A[static_cast<int>(P[i])]++;

    }

    int max = 0;
    int index = 0;

    for (int i = 33; i <= 100; i++)
    {
        if (A[i] > max)
        {
            max = A[i];
            index = i;
        }
    }

    for (int i = 33; i <= 100; i++)
    {
        if (i == index)
        {

        }
        else if (A[i] == max)
        {
            max = -1;
            break;
        }
    }

    if (max == -1)
        cout << "?";
    else
        cout << static_cast<char>(index);

    return 0;
}