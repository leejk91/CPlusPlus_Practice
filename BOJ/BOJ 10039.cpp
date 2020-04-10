#include <iostream>
using namespace std;

int main(void)
{
    int score[5] = { 0, };
    cin.tie(NULL);
    ios_base::sync_with_stdio(false);

    for (int i = 0; i <= 4; i++)
    {
        while (1)
        {
            cin >> score[i];
            if (score[i] >= 0 && score[i] <= 100 && score[i] % 5 == 0)
            {
                if (score[i] < 40)
                {
                    score[i] = 40;
                }
                break;
            }
            else
                continue;
        }
    }

    cout << (score[0] + score[1] + score[2] + score[3] + score[4]) / 5;

    return 0;
}