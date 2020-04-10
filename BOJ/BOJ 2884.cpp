#include <iostream>
using namespace std;

int main(void) {

    int H, M;

    while (1) {
        cin >> H;
        if (H >= 0 && H <= 23)
        {
            cin >> M;
            if (M >= 0 && M <= 59)
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

    if (M < 45) {
        M = 60 + (M - 45);
        if (H == 0) {
            H = 23;
        }
        else
            H = H - 1;
    }
    else
        M = M - 45;


    cout << H << " " << M;


    return 0;
}