#include <iostream>
using namespace std;

int main(void) {

    int A, B, C;

    while (1) {
        cin >> A >> B >> C;
        if (A >= 2 && B >= 2 && C >= 2 && A <= 10000 && B <= 10000 && C <= 10000)
            break;
        else
            continue;
    }
    cout << (A + B) % C << endl
        << ((A % C) + (B % C)) % C << endl
        << (A * B) % C << endl
        << ((A % C) * (B % C)) % C << endl;


}