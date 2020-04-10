#include <iostream>
using namespace std;

int main(void)
{

    int menu[5] = { 0, };
    int min = 0;
    cin.tie(NULL);
    ios_base::sync_with_stdio(false);

    for (int i = 0; i <= 4; i++)
    {
        while (1)
        {
            cin >> menu[i];
            if (menu[i] >= 100 && menu[i] <= 2000)
            {
                break;
            }
            else
                continue;
        }
    }
    if (menu[0] < menu[1])
    {
        if (menu[0] < menu[2])
            min = min + menu[0];
        else
            min = min + menu[2];
    }
    else
    {
        if (menu[1] < menu[2])
        {
            min = min + menu[1];
        }
        else
            min = min + menu[2];
    }

    if (menu[3] < menu[4])
    {
        min = min + menu[3];
    }
    else
        min = min + menu[4];

    cout << min - 50;

    return 0;
}