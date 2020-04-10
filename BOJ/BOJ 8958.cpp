#include <iostream>
#include <string>
using namespace std;

int main(void)
{
    // 백준 8958
    // 초기화
    int N = 0;
    string testcase;

    cin >> N;

    for (int i = 0; i < N; i++)
    {
        bool check = 0;
        int sum = 0;
        int com = 1;

        cin >> testcase;

        for (int i = 0; i <= testcase.length(); i++)
        {

            if (check == 0 && testcase[i] == 'O')
            {

                check = 1;
                sum = sum + com;
                com++;
            }
            else if (check == 1 && testcase[i] == 'O')
            {
                sum = sum + com;
                com++;
            }
            else if (check == 1 && testcase[i] == 'X')
            {
                com = 1;
                check = 0;
            }
            else if (check == 0 && testcase[i] == 'X')
            {
            }
        }
        cout << sum << "\n";
    }



    return 0;
}