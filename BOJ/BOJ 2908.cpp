#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

//백준 2908

int main(void)
{
    string A,B;

    cin >> A >> B;

    reverse(A.begin(), A.end());
    reverse(B.begin(), B.end());

    int AA = atoi(A.c_str());
    int BB = atoi(B.c_str());

    if (AA > BB)
        cout << AA;
    else
        cout << BB;

    return 0;
}