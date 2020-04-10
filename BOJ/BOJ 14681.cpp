#include <iostream>
using namespace std;

int main(void) {

    double x, y;

    while (1) {
        cin >> x;
        if ((x >= -1000 && x <= 1000) && x != 0)
        {
            cin >> y;
            if ((x >= -1000 && x <= 1000) && y != 0)
            {
                break;
            }
            else
            {
                continue;
            }
        }
        else
            continue;
    }

    if (x >= 0 && y >= 0)
    {
        cout << 1;
    }
    else if (x <= 0 && y >= 0)
    {
        cout << 2;
    }
    else if (x <= 0 && y <= 0)
    {
        cout << 3;
    }
    else if (x >= 0 && y <= 0)
    {
        cout << 4;
    }

    return 0;
}