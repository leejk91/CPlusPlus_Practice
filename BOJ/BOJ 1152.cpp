#include <iostream>
#include <string>
using namespace std;

//백준 1152

int main(void)
{
    string P;


    getline(cin, P);

    int size = P.size();

    int check = 0;
    int start = 0;

    for (int i = 0; i < size; i++)
    {
        if (start == 0)
        {
            if (static_cast<int>(P[i]) != 32)
            {
                start = 1;
                check++;
            }
        }
        else if (start == 1)
        {
            if (static_cast<int>(P[i]) == 32)
            {
                start=0;
            }
        }

    }

    cout << check;

    return 0;
}